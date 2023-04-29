// Discord get token script
// ⚠️ WARNING: DO NOT SHARE THE TOKEN YOU WILL GET WITH ANYONE ⚠️

// Press F12 to open the Developer Tools, then go to Console and paste the following:

// ==== Start copy ====
void(function () {
console.clear();
const dummyIframe = document.createElement('iframe');
document.head.append(dummyIframe);
const descriptor = Object.getOwnPropertyDescriptor(dummyIframe.contentWindow, 'localStorage');
dummyIframe.remove();

const recoveredStorage = descriptor.get.bind(window).call();

const getFromStorage = (key) => (
  (value = recoveredStorage.getItem(key)) ? JSON.parse(value) : null
);

const user_id = getFromStorage('user_id_cache');
const tokens = getFromStorage('tokens');

if (!tokens) console.log('Please log into your Discord account 🐢');
if (!user_id && tokens)  console.log(tokens ?? 'Failed to get token, please re login 🐢');
if (user_id && tokens)   console.log(tokens[user_id]);
}());
// ==== End copy ====
