import argparse
import logging
from ikomia import core

logger = logging.getLogger(__name__)


def test_cpp_base_class():
    logger.info("===== Test: I/O base class =====")
    # Default ctor
    io = core.CWorkflowTaskIO()
    logger.info(f"I/O name: {io.name}")
    logger.info(f"I/O data type: {io.data_type}")
    elt_count = io.get_unit_element_count()
    assert type(elt_count) == int
    data_available = io.is_data_available()
    assert data_available is False
    auto_input = io.is_auto_input()
    assert auto_input is False
    composite = io.is_composite()
    assert composite is False
    assert io.displayable is True
    assert io.auto_save is False
    io.clear_data()
    json_data = io.to_json()
    assert len(json_data) == 0

    # Ctor with data type
    io1 = core.CWorkflowTaskIO(core.IODataType.IMAGE)
    assert io1.data_type == core.IODataType.IMAGE
    io1.copy_static_data(io)

    # Ctor with data type and name
    io_name = "Mask input"
    io = core.CWorkflowTaskIO(core.IODataType.IMAGE_BINARY, io_name)
    assert io.data_type == core.IODataType.IMAGE_BINARY
    assert io.name == io_name

    # Attributes
    io.description = "My description"
    logger.info(f"I/O description: {io.description}")
    io.dim_count = 3
    logger.info(f"I/O dim count: {io.dim_count}")
    logger.info(f"I/O source path: {io.source_file_path}")

    # Print
    logger.info(io)


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'cpp_base_class' in running_tests:
        test_cpp_base_class()
