#!/usr/bin/env node

const prompt = require('password-prompt');

const TYPING_ENDPOINT = (channel_id) => `https://discord.com/api/v9/channels/${channel_id}/typing`;
const INTERVAL_DELAY = 4000;

// You can hardcode your token and channel_id here ⤵
const TOKEN = '';
const CHANNEL_ID = '';

async function ask(question) {
  try {
    const answer = await prompt(question, { method: 'hide' });
    return answer;
  } catch (e) {
    if (e?.message === 'SIGINT') {
      process.exit(1);
    }
  }
}

let already_print_success = false;

async function typingLoop({ token, channel_id }) {
  let response;
  try {
    response = await fetch(TYPING_ENDPOINT(channel_id), {
      method: 'POST',
      headers: { 'authorization': token }
    });
  }
  catch (e) {
    console.log('\nfetch() error:', e?.cause?.name ?? e);
    return;
  }

  if (response.status !== 204) {
    console.log('\nFailed to communicate with Discord ❎');
    console.log('code:', response.status);;
    return;
  }

  if (!already_print_success) {
    console.log('\nSuccessfully started typing 🐢');
    already_print_success = true;
  }

  setTimeout(typingLoop.bind(null, { token, channel_id }), INTERVAL_DELAY);
}

async function main() {
  const token      = TOKEN      || await ask('token: ');
  const channel_id = CHANNEL_ID || await ask('channel_id: ');

  typingLoop({ token, channel_id });
}

if (require.main === module) {
  main();
}
