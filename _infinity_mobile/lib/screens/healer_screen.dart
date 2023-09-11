import 'package:_infinity/reusable_widgets/reusable_widget.dart';
import 'package:_infinity/screens/home_screen.dart';
import 'package:flutter/material.dart';
import 'package:google_fonts/google_fonts.dart';
import 'dart:ui' as ui;
import 'dart:async';

class ObscuredTextFieldSample extends StatelessWidget {
  const ObscuredTextFieldSample({super.key});

  @override
  Widget build(BuildContext context) {
    return const SizedBox(
      width: 70,
      child: TextField(
        obscureText: true,
        decoration: InputDecoration(
          border: OutlineInputBorder(),
          labelText: 'HP',
        ),
      ),
    );
  }
}

class HealerScreen extends StatefulWidget {
  const HealerScreen({Key? key}) : super(key: key);

  @override
  _HealerScreenState createState() => _HealerScreenState();
}

class _HealerScreenState extends State<HealerScreen> {
  void initState() {
    main();
    super.initState();
  }

  main() async {
    Timer.periodic(Duration(seconds: 1), (timer) {
      socket.sendMessage("character-details-to-mobile");
    });
  }

  Widget _RefreshWidget() {
    return Padding(
      padding: const EdgeInsets.all(16.0),
      child: Column(
        children: <Widget>[
          ElevatedButton(
            child: Text('Refresh'),
            onPressed: () async {
              setState(() {});
            },
          ),
        ],
      ),
    );
  }

  @override
  Widget build(BuildContext context) {
    double screenWidth = MediaQuery.of(context).size.width;
    return Scaffold(
        body: Container(
            child: ListView(
          children: [
            _RefreshWidget(),
            Card(
                shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(20)),
                child: ListTile(
                  title: Text(
                    "Player Name",
                    style: GoogleFonts.lato(
                      fontSize: 18,
                      fontWeight: FontWeight.w500,
                    ),
                  ),
                  subtitle: Text(
                    "${message}",
                    style: GoogleFonts.lato(
                        fontSize: 15,
                        fontWeight: FontWeight.w500,
                        color: Colors.white),
                  ),
                  tileColor: Colors.green,
                  shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(20)),
                )),
            Card(
                shape: RoundedRectangleBorder(
                    borderRadius: BorderRadius.circular(20)),
                child: ListTile(
                  title: Text(
                    "Health Points",
                    style: GoogleFonts.lato(
                      fontSize: 18,
                      fontWeight: FontWeight.w500,
                    ),
                  ),
                  subtitle: Text(
                    "50",
                    style: GoogleFonts.lato(
                        fontSize: 15,
                        fontWeight: FontWeight.w500,
                        color: Colors.white),
                  ),
                  tileColor: Colors.green,
                  shape: RoundedRectangleBorder(
                      borderRadius: BorderRadius.circular(20)),
                )),
          ],
        )),
        drawer: dWidget());
  }
}

/*
Column(children: <Widget>[
          Row(
            //ROW 1
            children: [
              Container(
                //margin: EdgeInsets.all(25),
                child: ObscuredTextFieldSample(),
              ),
              Container(
                color: Colors.blue,
                margin: EdgeInsets.all(35),
                child: FlutterLogo(
                  size: 60.0,
                ),
              ),
              Container(
                color: Colors.purple,
                margin: EdgeInsets.all(35),
                child: FlutterLogo(
                  size: 60.0,
                ),
              ),
            ],
          ),
          Row(//ROW 2
              children: [
            Container(
              color: Colors.orange,
              margin: EdgeInsets.all(35),
              child: FlutterLogo(
                size: 60.0,
              ),
            ),
            Container(
              color: Colors.blue,
              margin: EdgeInsets.all(35),
              child: FlutterLogo(
                size: 60.0,
              ),
            ),
            Container(
              color: Colors.purple,
              margin: EdgeInsets.all(35),
              child: FlutterLogo(
                size: 60.0,
              ),
            )
          ]),
          Row(// ROW 3
              children: [
            Container(
              color: Colors.orange,
              margin: EdgeInsets.all(35),
              child: FlutterLogo(
                size: 60.0,
              ),
            ),
            Container(
              color: Colors.blue,
              margin: EdgeInsets.all(35),
              child: FlutterLogo(
                size: 60.0,
              ),
            ),
            Container(
              color: Colors.purple,
              margin: EdgeInsets.all(35),
              child: FlutterLogo(
                size: 60.0,
              ),
            ),
          ]),
        ])
        */


        /*

          Widget build(BuildContext context) {
    double screenWidth = MediaQuery.of(context).size.width;
    return Scaffold(
        body: Column(children: <Widget>[
          Row(
            children: [
              Container(
                  margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
                  alignment: Alignment.center,
                  height: 100,
                  width: screenWidth / 2,
                  child: ListTile(
                    title: Text(
                      'Player Name: asdasdasdasdasd',
                      style: GoogleFonts.lato(
                          fontSize: 12,
                          fontWeight: FontWeight.w500,
                          color: Colors.white),
                    ),
                  ),
                  decoration: BoxDecoration(
                      gradient: LinearGradient(
                          begin: Alignment.topRight,
                          end: Alignment.bottomLeft,
                          colors: [
                        Colors.red,
                        Colors.black,
                      ]))),
              Container(
                margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
                height: 100,
                width: screenWidth / 2,
                alignment: Alignment.center,
                child: Text("Player Level: "),
                color: Colors.blue,
              ),
            ],
          ),
          Row(
            children: [
              Container(
                margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
                height: 100,
                width: screenWidth / 2,
                alignment: Alignment.center,
                child: Text("Health Points: "),
                color: Colors.blue,
              ),
              Container(
                margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
                height: 100,
                width: screenWidth / 2,
                alignment: Alignment.center,
                child: Text("Mana Points: "),
                color: Colors.blue,
              ),
            ],
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Health Points Percentage: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Mana Points Percentage: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Fist Fighting Level: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Club Fighting Level: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Sword Fighting Level: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Axe Fighting Level: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Distance Fighting Level: "),
            color: Colors.blue,
          ),
          Container(
            margin: const EdgeInsets.fromLTRB(0, 0, 0, 0),
            alignment: Alignment.center,
            child: Text("Shielding Fighting Level: "),
            color: Colors.blue,
          ),
        ]),
        drawer: dWidget());
  }

  */