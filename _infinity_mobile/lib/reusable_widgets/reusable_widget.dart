import 'package:_infinity/screens/healer_screen.dart';
import 'package:_infinity/screens/home_screen.dart';
import 'package:flutter/material.dart';
import 'package:dart_jsonwebtoken/dart_jsonwebtoken.dart';
import 'package:google_fonts/google_fonts.dart';
import 'dart:ui' as ui;

import 'package:tcp_socket_connection/tcp_socket_connection.dart';

late String token;
String message = "";
String isAuth = "";
ValueNotifier<String> myString = ValueNotifier<String>('');

TcpSocketConnection socket = TcpSocketConnection("192.168.1.106", 8081);

String hs256(String email, String password) {
  /* Sign */
  // Create a json web token
  final jwt = JWT({
    'iss': 'mobile',
    'sub': 1234567890,
    'name': '${email} ${password}',
    'iat': '1516239022'
  });

  // Sign it

  return token = jwt.sign(SecretKey('secret'));
  //print('Signed token: $token\n');

  // /* Verify */ {
  //   try {
  //     // Verify a token
  //     final jwt = JWT.verify(token, SecretKey('secret'));

  //     print('Payload: ${jwt.payload}');
  //   } on JWTExpiredException {
  //     print('jwt expired');
  //   } on JWTException catch (ex) {
  //     print(ex.message); // ex: invalid signature
  //   }
  // }
}

Image logoWidget(String imageName) {
  return Image.asset(
    imageName,
    fit: BoxFit.fitWidth,
    width: 240,
    height: 240,
    color: Colors.white,
  );
}

TextField reusableTextField(String text, IconData icon, bool isPasswordType,
    TextEditingController controller) {
  return TextField(
    controller: controller,
    obscureText: isPasswordType,
    enableSuggestions: !isPasswordType,
    autocorrect: !isPasswordType,
    cursorColor: Colors.white,
    style: TextStyle(color: Colors.white.withOpacity(0.9)),
    decoration: InputDecoration(
      prefixIcon: Icon(
        icon,
        color: Colors.white70,
      ),
      labelText: text,
      labelStyle: TextStyle(color: Colors.white.withOpacity(0.9)),
      filled: true,
      floatingLabelBehavior: FloatingLabelBehavior.never,
      fillColor: Colors.white.withOpacity(0.3),
      border: OutlineInputBorder(
          borderRadius: BorderRadius.circular(30.0),
          borderSide: const BorderSide(width: 0, style: BorderStyle.none)),
    ),
    keyboardType: isPasswordType
        ? TextInputType.visiblePassword
        : TextInputType.emailAddress,
  );
}

Container UIButton(BuildContext context, String title, Function onTap) {
  return Container(
    width: MediaQuery.of(context).size.width,
    height: 50,
    margin: const EdgeInsets.fromLTRB(0, 10, 0, 20),
    decoration: BoxDecoration(borderRadius: BorderRadius.circular(90)),
    child: ElevatedButton(
      onPressed: () {
        onTap();
      },
      child: Text(
        title,
        style: const TextStyle(
            color: Colors.black87, fontWeight: FontWeight.bold, fontSize: 16),
      ),
      style: ButtonStyle(
          backgroundColor: MaterialStateProperty.resolveWith((states) {
            if (states.contains(MaterialState.pressed)) {
              return Colors.black26;
            }
            return Colors.white;
          }),
          shape: MaterialStateProperty.all<RoundedRectangleBorder>(
              RoundedRectangleBorder(borderRadius: BorderRadius.circular(30)))),
    ),
  );
}

int _selectedIndex = 0;

class dWidget extends StatefulWidget {
  const dWidget({Key? key}) : super(key: key);

  @override
  _DWidgetState createState() => _DWidgetState();
}

class _DWidgetState extends State<dWidget> {
  void initState() {
    startConnection();
    super.initState();
  }

  void messageReceived(String msg) {
    setState(() {
      message = msg;
    });
  }

  //starting the connection and listening to the socket asynchronously
  void startConnection() async {
    socket.enableConsolePrint(
        true); //use this to see in the console what's happening
    if (await socket.canConnect(5000, attempts: 3)) {
      //check if it's possible to connect to the endpoint
      await socket.connect(5000, messageReceived, attempts: 3);
    }
  }

  void _onItemTapped(int index) {
    setState(() {
      _selectedIndex = index;
    });
  }

  Widget _body() {
    switch (_selectedIndex) {
      case 0:
        return GestureDetector(
            onTap: () {
              setState(() {
                _selectedIndex = 0;
              });
            },
            child: const HealerScreen());
      default:
        return GestureDetector(
            onTap: () {
              setState(() {
                _selectedIndex = 0;
              });
            },
            child: const HealerScreen());
    }
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
          title: Text('Infinity',
              style: GoogleFonts.lato(
                fontSize: 25,
                fontWeight: FontWeight.w500,
              ))),
      body: _body(),
      drawer: Drawer(
        // Add a ListView to the drawer. This ensures the user can scroll
        // through the options in the drawer if there isn't enough vertical
        // space to fit everything.
        child: ListView(
          // Important: Remove any padding from the ListView.
          padding: EdgeInsets.zero,
          children: <Widget>[
            DrawerHeader(
              child: Text(''),
              decoration: BoxDecoration(
                  color: Colors.blue,
                  image: DecorationImage(
                      image: AssetImage("assets/images/infinity.png"),
                      scale: 0.30)),
            ),
            ListTile(
              title: Text(
                'Player Information',
                style: GoogleFonts.lato(
                  foreground: Paint()
                    ..shader = ui.Gradient.linear(
                      const Offset(0, 20),
                      const Offset(270, 20),
                      <Color>[
                        Colors.blue,
                        Colors.purple,
                      ],
                    ),
                  fontSize: 20,
                  fontWeight: FontWeight.w500,
                ),
              ),
              selected: _selectedIndex == 0,
              leading: Icon(
                Icons.account_circle_rounded,
                size: 45,
                color: Colors.purple,
              ),
              tileColor: Colors.blue,
              onTap: () {
                // Update the state of the app
                _onItemTapped(0);
                // Then close the drawer
                Navigator.pop(context);
                // Navigator.push(
                //   context,
                //   MaterialPageRoute(builder: (context) => HealerScreen()),
                // );
              },
            ),
            ListTile(
              title: Text(
                'Logout',
                style: GoogleFonts.lato(
                  foreground: Paint()
                    ..shader = ui.Gradient.linear(
                      const Offset(0, 20),
                      const Offset(170, 20),
                      <Color>[
                        Colors.blue,
                        Colors.purple,
                      ],
                    ),
                  fontSize: 20,
                  fontWeight: FontWeight.w500,
                ),
              ),
              selected: _selectedIndex == 0,
              leading: Icon(
                Icons.logout,
                size: 45,
                color: Colors.purple,
              ),
              tileColor: Colors.blue,
              onTap: () {
                // Update the state of the app
                _onItemTapped(0);
                // Then close the drawer
                Navigator.pop(context);
                // Navigator.push(
                //   context,
                //   MaterialPageRoute(builder: (context) => HealerScreen()),
                // );
              },
            )
          ],
        ),
      ),
    );
  }
}

/*
TextStyle(
                    fontStyle: FontStyle.italic,
                    color: Colors.bue,l
                    fontSize: 20,
                    fontWeight: FontWeight.w400),

                    */
