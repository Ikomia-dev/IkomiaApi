import os.path

from ikomia import utils, core, dataprocess
from ikomia.core import config
from urllib.parse import urlparse


class Workflow(dataprocess.CWorkflow):

    def __init__(self, name="Untitled", registry=None):
        if registry is None:
            dataprocess.CWorkflow.__init__(self, name)
        else:
            dataprocess.CWorkflow.__init__(self, name, registry)

        output_folder = config.main_cfg["workflow"]["path"]
        self.setOutputFolder(output_folder)

    def set_image_input(self, array=None, path="", url="", index=-1, datatype=core.IODataType.IMAGE):
        if array is not None:
            img_input = dataprocess.CImageIO(datatype, array)
        elif path:
            img_input = dataprocess.CImageIO(datatype, path)
        elif url:
            parsed = urlparse(url)
            img_path = config.main_cfg["data"]["path"] + os.path.basename(parsed.path)
            utils.http.download_file(url, img_path)
            img_input = dataprocess.CImageIO(datatype, img_path)

        if index == -1:
            self.addInput(img_input)
        else:
            self.setInput(img_input, index, True)

    def run(self):
        self.updateStartTime()
        super().run()
