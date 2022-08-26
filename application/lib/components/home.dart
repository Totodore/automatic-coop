import 'package:automatic_coop_app/bluetooth.dart';
import 'package:automatic_coop_app/components/ble_failed.dart';
import 'package:automatic_coop_app/components/home_app_bar.dart';
import 'package:flutter/material.dart';
import 'package:logger/logger.dart';

extension on DateTime {
  String toLocaleString() {
    return "${day.toString().padLeft(2, '0')}/${month.toString().padLeft(2, '0')}/${year.toString().padLeft(2, '0')} - ${hour.toString().padLeft(2, '0')}:${minute.toString().padLeft(2, '0')}:${second.toString().padLeft(2, '0')}";
  }
  String toLocaleHourString() {
    return "${hour.toString().padLeft(2, '0')}:${minute.toString().padLeft(2, '0')}:${second.toString().padLeft(2, '0')}";
  }
}
class Home extends StatefulWidget {
  Home({Key? key}) : super(key: key);

  final Bluetooth bluetooth = Bluetooth();
  final Logger logger = Logger();

  @override
  State<StatefulWidget> createState() => _HomeState();

}
class _HomeState extends State<Home> {

  bool isLoading = true;

  Bluetooth get bluetooth => widget.bluetooth;

  String get title => isLoading ? "Recherche d'un poulailler..." : bluetooth.name;

  double? temp;

  bool? doorOpen;
  bool? doorFree;

  @override
  void initState() {
    super.initState();
    init();
  }

  void init() async {
    if (await _connect()) {
      doorOpen = await bluetooth.getDoorState();
      doorFree = await bluetooth.getDoorFree();
      setState(() { });
    }
  }

  @override
  void dispose() async {
    super.dispose();
    widget.logger.i("Closing home, disconnecting bluetooth");
    await bluetooth.disconnect();
    widget.logger.i("Bluetooth disconnected");
  }

  Future<bool> _connect() async {
    if (bluetooth.isConnected) {
      await bluetooth.disconnect();
    }
    if (await bluetooth.connect()) {
      setState(() => isLoading = false);
      return true;
    } else {
      showDialog(context: context, builder: (_) => BleFailed(() {
        Navigator.of(context).pop();
        // _connect();
      }), barrierDismissible: false);
      return false;
    } 
  }

  void _updateTime() async {
    if (bluetooth.isConnected) {
      await bluetooth.setCurrentTime();
    } else {
      setState(() {});
    }
  }

  void _toggleDoor() async {
    if (bluetooth.isConnected) {
      if (doorOpen!) {
        await bluetooth.closeDoor();
      } else {
        await bluetooth.openDoor();
      }
      doorOpen = await bluetooth.getDoorState();
      doorFree = await bluetooth.getDoorFree();
    }
    setState(() { });
  }

  void _freeDoor() async {
    if (bluetooth.isConnected) {
      await bluetooth.freeDoor();
    }
    doorFree = await bluetooth.getDoorFree();
    doorOpen = await bluetooth.getDoorState();
    setState(() { });
  }

  Widget _info(IconData icon, String? text) => text != null 
    ? ListTile(
      leading: Icon(icon, color: Colors.lightGreen, size: 30), 
      title: Text(text, style: const TextStyle(fontSize: 19))
    )
    : const SizedBox.shrink();

  Widget _divider() => const Divider(color: Colors.grey);
  
  @override
  Widget build(BuildContext context) => Scaffold(
    appBar: HomeAppbar(title, () => isLoading ? null : init()),
    body: Column(
      children: [
        isLoading ? const LinearProgressIndicator(minHeight: 5) : const SizedBox.shrink(),
        bluetooth.isConnected ? Padding(
          padding: const EdgeInsets.symmetric(vertical: 15, horizontal: 15),
          child: Column(
            children: [
              doorOpen != null ? _info(!doorOpen! ? Icons.door_back_door : Icons.door_back_door_outlined, doorOpen! ? "Porte ouverte" : "Porte fermée") : const SizedBox.shrink(),
              doorOpen != null ? Row(mainAxisSize: MainAxisSize.min, 
                children: [
                  Expanded(
                    child: Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 4.0),
                      child: ElevatedButton(
                        onPressed: doorOpen == null ? null : () => _toggleDoor(), 
                        child: Text(doorOpen == null ? "Chargement..." : doorOpen! ? "Fermer la porte" : "Ouvrir la porte", style: const TextStyle(color: Colors.white)),
                      ),
                    ),
                  ),
                  doorFree != null ? Expanded(
                    child: Padding(
                      padding: const EdgeInsets.symmetric(horizontal: 4.0),
                      child: ElevatedButton(
                        child: const Text("Libérer la porte", style: TextStyle(color: Colors.white)), 
                        onPressed: doorFree! ? null : () => _freeDoor(),
                      ),
                    ),
                  ) : const SizedBox.shrink(),
                ]
              ) : const SizedBox.shrink(),
              _divider(),
              FutureBuilder<double>(
                builder: (_, snapshot) => _info(Icons.thermostat, snapshot.hasData ? "Température: ${snapshot.data}°C" : null),
                future: temp != null ? Future.value(temp) : bluetooth.getTemperature().then((value) => temp = value)
              ),
              StreamBuilder<DateTime?>(
                builder: (_, snapshot) => _info(Icons.schedule, snapshot.hasData ? snapshot.data?.toLocaleString() : null),
                stream: Stream.periodic(const Duration(seconds: 1))
                  .asyncMap((_) async => bluetooth.getCurrentTime())
              ),
              ElevatedButton(
                onPressed: () => bluetooth.isConnected ? _updateTime() : null, 
                child: const Text("Mettre à l'heure du téléphone", style: TextStyle(color: Colors.white)),
              ),
              _divider(),
              FutureBuilder<DateTime?>(
                builder: (_, snapshot) => _info(Icons.wb_sunny_outlined, snapshot.hasData ? "Ouverture : ${snapshot.data!.toLocaleHourString()}" : null), 
                future: bluetooth.getSunriseTime() 
              ),
              FutureBuilder<DateTime?>(
                builder: (_, snapshot) => _info(Icons.nightlight_outlined, snapshot.hasData ? "Fermeture : ${snapshot.data!.toLocaleHourString()}" : null),
                future: bluetooth.getSunsetTime()
              )
            ],
          ),
        ) : const SizedBox.shrink(),
      ],
    ),
  );
}
