from ikomia.dnn import monitoring


if __name__ == "__main__":
    # Same startup logic as training tasks: SQLite backend, legacy runs migration, health check
    monitoring.check_mlflow_server()
    monitoring.check_tensorboard_server()
