import 'package:flutter/material.dart';

class BleFailed extends StatelessWidget {
  final void Function() onPressed;
  const BleFailed(this.onPressed, {Key? key}) : super(key: key);

  @override
  Widget build(BuildContext context) => AlertDialog(
    title: const Text("Connection bluetooth impossible"),
    actions: [
      ElevatedButton(onPressed: onPressed, child: const Text("Réessayer", style: TextStyle(color: Colors.white))),
    ],
  );

}