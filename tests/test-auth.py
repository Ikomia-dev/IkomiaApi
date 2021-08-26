import logging
import ikomia

logger = logging.getLogger(__name__)


if __name__ == "__main__":
    logger.info("===== Test::check authentication =====")
    ikomia.authenticate()

    if ikomia.ik_api_session is not None:
        logger.info("Session token: " + str(ikomia.ik_api_session.token))
    else:
        logger.error("Authentication failed, please check credentials.")
