"""Prints the version of the app for use in container builds."""

from datetime import datetime
import os

if "GITHUB_REF_TYPE" in os.environ and os.environ["GITHUB_REF_TYPE"] == "tag":
    version = os.environ["GITHUB_REF_NAME"]
else:
    version = datetime.now().isoformat().replace(":", "-")
print(version)
