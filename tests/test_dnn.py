import ikomia.dnn as dnn


def test_monitoring():
    dnn.monitoring.check_mlflow_server()
    dnn.monitoring.check_tensorboard_server()


if __name__ == "__main__":
    test_monitoring()
