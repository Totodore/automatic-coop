import 'package:automatic_coop_app/components/home.dart';
import 'package:flutter/material.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Poulailler',
      theme: ThemeData(
        primarySwatch: Colors.lightGreen,
        textTheme: const TextTheme(
          bodyText1: TextStyle(color: Colors.white),
        )
      ),
      home: Home(),
    );
  }
}
