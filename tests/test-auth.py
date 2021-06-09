from ikomia.core import auth


if __name__ == "__main__":
    auth.authenticate("Ludo", "ludo?imageez")
    print(auth.api_token)
