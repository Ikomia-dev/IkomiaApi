import argparse
import logging
from ikomia.core import CWorkflowTaskParam

logger = logging.getLogger(__name__)


def test_set_values():
    logger.warning("===== Test: set values =====")
    param_obj = CWorkflowTaskParam()

    # Set parameters with valid dict
    values = {"param1": "value1", "param2": "value2", "param3": "value3"}
    param_obj.set_values(values)
    logger.warning(param_obj)

    # Set parameters with mixed-type dict
    values = {"param1": 0, "param2": "value2", "param3": 1.0}
    try:
        param_obj.set_values(values)
    except TypeError as e:
        logger.error(e)


def test_get_values():
    logger.warning("===== Test: get values =====")
    param_obj = CWorkflowTaskParam()
    values = param_obj.get_values()
    assert(type(values) == dict)
    assert(len(values) == 0)

    # Set parameters with valid dict
    values = {"param1": "value1", "param2": "value2", "param3": "value3"}
    param_obj.set_values(values)
    values = param_obj.get_values()
    assert (type(values) == dict)
    assert (len(values) == 3)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'set_values' in running_tests:
        test_set_values()
    if 'all' in running_tests or 'get_values' in running_tests:
        test_get_values()
