"""Workflow class to manage LLM-based workflows."""
import json
import logging
from pathlib import Path
from typing import Union, Literal, Optional

import yaml

from ikomia.core.enums import WorkflowType
from ikomia.dataprocess.registry import IkomiaRegistry, ik_registry
from ikomia.dataprocess.workflow.base import Workflow

logger = logging.getLogger(__name__)


class LlmWorkflow(Workflow):
    """LLM-based workflow intended for serving through dedicated backend framework."""

    _to_backend_keys = {
        "vllm": {
            "model_name": "model",
        },
        "sglang": {
            "model_name": "model_path",
        }
    }

    def __init__(
        self,
        name: str = "Untitled",
        backend: Literal["vllm", "sglang"] = "vllm",
        model_name: str = "",
        config_file_path: str = "",
        registry: IkomiaRegistry = ik_registry,
    ):
        """
        Initialize LLM workflow.

        Args:
            name (str): workflow name
            backend (str): serving framework (vLLM, SGLang)
            model_name (str): LLM model to serve
            config_file_path (str): path to custom server configuration
            registry (IkomiaRegistry): algorithms registry
        """
        super().__init__(name, registry)
        self.type = WorkflowType.LLM
        self.backend = backend
        self.config = None

        if config_file_path:
            self.load_config(config_file_path)
        else:
            self._load_default_config()

        if model_name and self.config:
            self.config[self._to_backend_keys[backend]["model_name"]] = model_name

    def load_config(self, path: Union[str, Path]):
        """
        Load custom serving framework configuration.

        Args:
            path (str): path to configuration file (.yaml)
        """
        with open(path, "r", encoding="utf-8") as f:
            self.config = yaml.safe_load(f)

    def _load_default_config(self):
        """Internal use only."""
        config_path = (
            Path(__file__)
            .resolve()
            .parent.joinpath(f"config/{self.backend}_default.yaml")
        )
        self.load_config(config_path)

    def set_backend_parameters(self, **kwargs):
        """
        Override configuration parameters.

        Args:
            **kwargs: pairs of key-value, key must be a valid parameter.
        """
        if self.config:
            for key, value in kwargs.items():
                self.config[key] = value

    def load(self, path: str):
        """
        Load LLM workflow from definition file (.json).

        LLM workflow can come with a backend configuration file.
        The function searches for configuration file with same name in the same folder and load it if found.

        Args:
            path (str): path to workflow definition file (.json)
        """
        super().load(path)

        with open(path, "r", encoding="utf-8") as f:
            json_wf = json.load(f)
            if "llm" in json_wf:
                self.backend = json_wf["llm"]["backend"]
                config_file_name = json_wf["llm"]["config_file"]
                wf_path = Path(path)
                config_path = wf_path.parent.joinpath(config_file_name)

                if config_path.exists():
                    logger.info("Find custom LLM backend config: %s", config_path)
                    self.load_config(config_path)

    def save(
        self,
        path: str,
        exposed_params: Optional[dict] = None,
        exposed_outputs: Optional[dict] = None,
    ):
        """
        Save LLM workflow as a JSON definition file.

        The backend configuration (.yaml) is automatically saved in the same folder and with the same name.

        Args:
            path (str): path where the workflow file will be saved
            exposed_params (dict): list of task parameters that has to be exposed at workflow level
            exposed_outputs (dict): list of outputs that has to be exposed at workflow level
        """
        if exposed_params:
            self._expose_parameters(exposed_params)

        if exposed_outputs:
            self._expose_outputs(exposed_outputs)

        wf_path = Path(path)
        config_path = wf_path.parent.joinpath(
            f"{wf_path.stem}_{self.backend}_config.yaml"
        )
        wf_json = json.loads(self.to_json())
        wf_json["llm"] = {
            "backend": self.backend,
            "config_file": config_path.name,
        }

        with (
            open(path, "w", encoding="utf-8") as f,
            open(config_path, "w", encoding="utf-8") as fconfig
        ):
            json.dump(wf_json, f)
            yaml.dump(self.config, fconfig)
