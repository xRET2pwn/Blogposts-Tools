This Python script retrieves the ClipboardPayload data from the ActivityOperation table of the Connected Devices Platform SQLite database for each user profile on a Windows system. It first uses the psutil module to get a list of user names on the system, and then searches for the Connected Devices Platform folder in each user's AppData/Local directory. It then searches for any SQLite database files in the subdirectories of the Connected Devices Platform folder and retrieves the ClipboardPayload data from the ActivityOperation table of each database. If the ClipboardPayload data is in the Base64-encoded format, it is decoded and printed to the console.


You can convert it to exe through the following commands
```py
pip install pyinstaller
pyinstaller --onefile .\ClipboardHistorySniper.py
.\dist\ClipboardHistorySniper.exe
```
