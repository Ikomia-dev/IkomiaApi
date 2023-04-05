# Copyright (C) 2021 Ikomia SAS
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

"""
Module providing Ikomia workflow I/O implementation for data stored as Python dict.
"""

from ikomia.core import CWorkflowTaskIO, IODataType
import json


class DataDictIO(CWorkflowTaskIO):
    """
    Class implementing Ikomia workflow input/output object where data is stored as generic Python dict.
    Please note that such unformatted data are meant to be used as task output essentially.
    For complete compatibility with other API features, you must ensure that the dict content is fully
    JSON serializable. Inherit :py:class:`~ikomia.core.pycore.CWorkflowTaskIO`.

    :ivar data: data dictionary
    """
    def __init__(self):
        CWorkflowTaskIO.__init__(self, IODataType.DATA_DICT)
        self.data = {}

    def clear_data(self):
        """
        Clear whole data dict.
        """
        self.data.clear()

    def is_data_available(self):
        """
        Check whether the dataset structure contains data.

        Return:
            boolean: True or False
        """
        return len(self.data) > 0

    def save(self, path):
        """
        Save data dict as JSON.

        Args:
            path: file path where data is saved
        """
        with open(path, "w") as outfile:
            json.dump(self.data, outfile)

    def load(self, path):
        """
        Load JSON as data dict I/O.

        Args:
            path: file path where dataset is saved
        """
        with open(path, "r") as infile:
            self.data = json.load(infile)

    def to_json(self, options=[]):
        json_format = "compact"
        if "json_format" in options:
            json_format = options[options.index("json_format") + 1]

        if json_format == "indented":
            return json.dumps(self.data, indent=4)
        else:
            return json.dumps(self.data)

    def from_json(self, json_string):
        self.data = json.loads(json_string)
