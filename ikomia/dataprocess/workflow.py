from ikomia import core, dataprocess


class Workflow(dataprocess.CWorkflow):

    def __init__(self, name="Untitled", registry=None):
        if registry is None:
            dataprocess.CWorkflow.__init__(self, name)
        else:
            dataprocess.CWorkflow.__init__(self, name, registry)

    def add_image_input(self, path, type=core.IODataType.IMAGE):
        input = dataprocess.CImageIO(type, path)
        self.addInput(input)
