import logging
import ikomia

logger = logging.getLogger(__name__)


if __name__ == "__main__":
    logger.info("===== Test::check authentication =====")
    ikomia.initialize()
    print(ikomia.api_session.token)
