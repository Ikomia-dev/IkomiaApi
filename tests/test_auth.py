import logging
import argparse
import os
import ikomia
from dotenv import load_dotenv

logger = logging.getLogger(__name__)


def test_authenticate_from_env():
    logger.info("===== Test::authentication from env =====")
    ikomia.authenticate()
    assert ikomia.ik_api_session is not None
    assert ikomia.ik_api_session.token is not None


def test_authenticate_from_params():
    logger.info("===== Test::authentication from env =====")
    load_dotenv()
    username = os.environ.get("IKOMIA_USER")
    password = os.environ.get("IKOMIA_PWD")
    ikomia.authenticate(username=username, password=password)
    assert ikomia.ik_api_session is not None
    assert ikomia.ik_api_session.token is not None

    token = os.environ.get("IKOMIA_TOKEN")
    ikomia.authenticate(token=token)
    assert ikomia.ik_api_session is not None
    assert ikomia.ik_api_session.token is not None


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--tests",
                        type=str,
                        default='all',
                        help="List of tests to execute (comma-separated string, default=all)")

    opt = parser.parse_args()
    running_tests = opt.tests.split(',')

    if 'all' in running_tests or 'auth_from_env' in running_tests:
        test_authenticate_from_env()
    if 'all' in running_tests or 'auth_from_params' in running_tests:
        test_authenticate_from_params()
