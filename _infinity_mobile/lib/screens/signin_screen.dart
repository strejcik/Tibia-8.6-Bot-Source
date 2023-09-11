import 'package:firebase_auth/firebase_auth.dart';
import 'package:_infinity/reusable_widgets/reusable_widget.dart';
import 'package:_infinity/screens/home_screen.dart';
import 'package:_infinity/screens/reset_password.dart';
import 'package:_infinity/screens/singup_screen.dart';
import 'package:_infinity/utils/color_utils.dart';
import 'package:flutter/material.dart';

class SignInScreen extends StatefulWidget {
  const SignInScreen({Key? key}) : super(key: key);
  @override
  _SignInScreenState createState() => _SignInScreenState();
}

class _SignInScreenState extends State<SignInScreen> {
  TextEditingController _passwordTextController = TextEditingController();
  TextEditingController _emailTextController = TextEditingController();

  void initState() {
    super.initState();
    startConnection();

    myString.addListener(() => token = myString.value);

    // print value on change
    myString.addListener(() => print(myString.value));
    // do stuff
    //myString.addListener(myFunction)
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

  // void dispose() {
  //   socket.disconnect();
  //   socket.dispose();
  //   super.dispose();
  // }

  main() async {
    // Socket socket = await Socket.connect('192.168.1.106', 8081);

    // print('connected');

    // socket.listen((List<int> event) {
    //   print(utf8.decode(event));
    // });

    // socket.add(utf8.encode('hello'));

    // socket.connect();
    // socket.onConnect((_) {
    //   print('Connection established');
    // });
    // socket.onDisconnect((_) => Navigator.push(
    //     context, MaterialPageRoute(builder: (context) => SignInScreen())));
    // socket.onConnectError((err) => print(err));
    // socket.onError((err) => print(err));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        width: MediaQuery.of(context).size.width,
        height: MediaQuery.of(context).size.height,
        decoration: BoxDecoration(
            gradient: LinearGradient(colors: [
          hexStringToColor("1e90FF"),
          hexStringToColor("0072E1"),
        ], begin: Alignment.topCenter, end: Alignment.bottomCenter)),
        child: SingleChildScrollView(
          child: Padding(
            padding: EdgeInsets.fromLTRB(
                20, MediaQuery.of(context).size.height * 0.2, 20, 0),
            child: Column(
              children: <Widget>[
                logoWidget("assets/images/infinity.png"),
                const SizedBox(
                  height: 30,
                ),
                reusableTextField("Enter UserName", Icons.person_outline, false,
                    _emailTextController),
                const SizedBox(
                  height: 20,
                ),
                reusableTextField("Enter Password", Icons.lock_outline, true,
                    _passwordTextController),
                const SizedBox(
                  height: 5,
                ),
                forgetPassword(context),
                UIButton(context, "Sign In", () async {
                  token =
                      "mobile-token?${hs256(_emailTextController.text, _passwordTextController.text)}";
                  socket.sendMessage(token);
                  await fetchSocketResponse();
                  //socket.on('event', (data) => print(data));
                  // FirebaseAuth.instance
                  //     .signInWithEmailAndPassword(
                  //         email: _emailTextController.text,
                  //         password: _passwordTextController.text)
                  //     .then((value) {
                  //   Navigator.push(context,
                  //       MaterialPageRoute(builder: (context) => HomeScreen()));
                  // }).onError((error, stackTrace) {
                  //   print("Error ${error.toString()}");
                  // });
                }),
                signUpOption()
              ],
            ),
          ),
        ),
      ),
    );
  }

  Future<bool> fetchSocketResponse() =>
      Future.delayed(Duration(seconds: 1), () {
        if (message == "unauthorized") {
          Navigator.push(
              context, MaterialPageRoute(builder: (context) => SignInScreen()));
        }

        if (message == "authorized") {
          Navigator.push(
              context, MaterialPageRoute(builder: (context) => HomeScreen()));
        }

        return true;
      });

  Row signUpOption() {
    return Row(
      mainAxisAlignment: MainAxisAlignment.center,
      children: [
        const Text("Don't have account?",
            style: TextStyle(color: Colors.white70)),
        GestureDetector(
          onTap: () {
            Navigator.push(context,
                MaterialPageRoute(builder: (context) => SignUpScreen()));
          },
          child: const Text(
            " Sign Up",
            style: TextStyle(color: Colors.white, fontWeight: FontWeight.bold),
          ),
        )
      ],
    );
  }

  Widget forgetPassword(BuildContext context) {
    return Container(
      width: MediaQuery.of(context).size.width,
      height: 35,
      alignment: Alignment.bottomRight,
      child: TextButton(
        child: const Text(
          "Forgot Password?",
          style: TextStyle(color: Colors.white70),
          textAlign: TextAlign.right,
        ),
        onPressed: () => Navigator.push(
            context, MaterialPageRoute(builder: (context) => ResetPassword())),
      ),
    );
  }
}
