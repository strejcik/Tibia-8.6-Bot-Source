# Tibia-8.6-Bot-Source

### Features

![](https://i.imgur.com/mkDEK5A.png)


![](https://i.imgur.com/Or0fEjV.png)


![](https://i.imgur.com/QtpVCUk.png)


![](https://i.imgur.com/0mSsnwY.png)


![](https://i.imgur.com/xhesZgV.png)


![](https://i.imgur.com/a8XS0iT.png)


![](https://i.imgur.com/YHMl46g.png)


![](https://i.imgur.com/H6SZ2Qo.png)


![](https://i.imgur.com/02Ve6ma.png)


### Information

- "Infinity.exe" and "Infinity.dll" are 100% clean and free of viruses. Anti-virus might claim that those files are malicious/dangerous it's because there is reading/writing in Tibia memory. It's false positive. Also if you have your anti virus running it might block "Infinity.exe" or "Infinity.dll", so you can add those 2 files for exceptions in anti-virus settings.

- Bot probably is not compatibile with older version of Windows. Works fine with Windows 10

- Infinity Bot(Infinity.exe & Infinity.dll), _infinity_server, _infinity_mobile had been written only for educational purposes- mainly to learn more about software development. There is no guarantee that bot is 100% safe to use. If you are unsure if you want to use the bot, then please don't.
I don't take resposibility for misuse / incorrect use / possible ban in game / losses or in general use of this piece of software. You use it on your own, it's your choice.

### Basic Usage

`Run Tibia 8.6`

`Run Infinity.exe`

### Installing and running Infinity bot + server + mobile app

#### Installation part:

`Infinity.sln`: If you want to use bot along with server and mobile app you have to do few things. At the beginning you have to have installed boost asio either vcpkg or manually. The necessary boost asio dll files have to be linked to the Infinity.sln. Then you have to set up in Infinity.sln correct ip address and port- you will use same socket port and ip for bot, server and mobile app. Set mobileMode variable to true, then compile the project.

`_infinity_server`:
`npm install --save`, then set correct ip address and port in index.js.

`_infinity_mobile`: You have to have installed flutter, VSC and Android Studio. Change the ip and port to the correct one as been mentioned before.

#### Running bot, server and mobile app together:

1.`infinity_server: npm start`
2.`Run Infinity.exe`
3.`_infinity_mobile: you can either build the flutter app and upload it to mobile phone or you can run the mobile app as a virtual device in debug mode.`

Since all the things are running, in mobile app you should login with credentials:
login: john 
password: doe
Then inside game cast a spell, for example 'exura'. In mobile app press refresh. All details should be fetched.



