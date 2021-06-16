import ikomia


if __name__ == "__main__":
    ikomia.initialize("Ludo", "toto")
    ikomia.initialize("Ludo", "ludo?imageez")
    print(ikomia.api_session.token)
