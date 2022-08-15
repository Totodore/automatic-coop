import 'package:flutter/material.dart';

class HomeAppbar extends StatelessWidget implements PreferredSizeWidget {
  final String title;
  final void Function()? onRefresh;
  const HomeAppbar(this.title, this.onRefresh, {Key? key}) : super(key: key);
  @override
  Widget build(BuildContext context) => AppBar(
      title: Text(title, style: const TextStyle(color: Colors.white)),
      leading: Padding(
        padding: const EdgeInsets.all(8.0),
        child: Image.asset('assets/chicken-coop.png', fit: BoxFit.contain, height: 10),
      ),
      actions: [
        IconButton(
          icon: const Icon(Icons.refresh, color: Colors.white),
          onPressed: onRefresh,
        ),
      ],
    );

  @override
  Size get preferredSize => const Size.fromHeight(kToolbarHeight);
}