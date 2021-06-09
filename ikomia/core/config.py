import os
import yaml

global main_cfg

directory = os.path.dirname(os.path.abspath(__file__))
main_cfg = yaml.safe_load(open(directory + "/config.yaml"))