import logging
import ikomia

logger = logging.getLogger(__name__)


def test_authenticate_from_env():
    logger.info("===== Test::authentication from env =====")
    ikomia.authenticate()
    assert ikomia.ik_api_session is not None


if __name__ == "__main__":
    test_authenticate_from_env()
