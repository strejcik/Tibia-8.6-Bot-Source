import 'package:_infinity/reusable_widgets/reusable_widget.dart';
import 'package:_infinity/screens/healer_screen.dart';
import 'package:firebase_auth/firebase_auth.dart';
import 'package:_infinity/screens/signin_screen.dart';
import 'package:flutter/material.dart';

class HomeScreen extends StatefulWidget {
  const HomeScreen({Key? key}) : super(key: key);

  @override
  _HomeScreenState createState() => _HomeScreenState();
}

class _HomeScreenState extends State<HomeScreen> {
  void initState() {
    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Container(
        // child: _widgetOptions[_selectedIndex],
        child: dWidget(),
      ),
    );
  }
}
