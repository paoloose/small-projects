
# Discord typing script

## Usage

First you will need your **Discord token** and the **channel id** you want to
type in.

Your token is stored inside the Discord local storage. Go to Developer Tools >
Application > Local Storage > 'https://discord.com' > tokens.

Or simply run the [get_token_script.js](./get_token_script.js) in your web
browser console (F12 > Console) and copy the token.

You can extract the channel id from the URL, it has the format
`https://discord.com/channels/<guild_id>/<channel_id>`.

Now you can run the script:

```bash
# Install the dependencies
npm install

# Run the script
npm start
```

You will be prompted to enter your token and the channel id.

To avoid being prompted, you can hardcode those values in the
[main.js](./main.js) script or passing them as arguments:

```bash
# Run the script with arguments
npm start -- <token> <channel_id>
```
