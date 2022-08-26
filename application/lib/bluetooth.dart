import 'dart:async';
import 'dart:math';
import 'dart:typed_data';

import 'package:flutter/foundation.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

import 'package:automatic_coop_app/flags.dart';
import 'package:logger/logger.dart';
import 'package:shared_preferences/shared_preferences.dart';


extension on Uint8List {
  String toHexString() {
    final sb = StringBuffer();
    for (final byte in this) {
      sb.write('${byte.toRadixString(16).padLeft(2, '0')}-');
    }
    return sb.toString();
  }
}


class Bluetooth {
  
  static final Bluetooth _instance = Bluetooth._();

  factory Bluetooth() => _instance;

  Bluetooth._();

  final Logger logger = Logger(printer: SimplePrinter(printTime: true));
  String? _name;
  String? _address;

  BluetoothConnection? _connection;

  List<Packet> _buffer = [];  

  Future<bool> connect() async {
    FlutterBluetoothSerial.instance.onStateChanged().listen((state) {
      logger.d('Bluetooth state changed: $state');
    });
    await FlutterBluetoothSerial.instance.requestEnable();
    await _getPreviousDevice();

    if (_address == null || !await _connectToPreviousDevice()) {
      try {
        BluetoothDiscoveryResult result = await FlutterBluetoothSerial.instance.startDiscovery()
          // .timeout(const Duration(seconds: 20))
          // .map((el) {
          //   logger.i('Device: ${el.device.name}');
          //   return el;
          // })
          .singleWhere((element) => element.device.name?.startsWith("Poulailler") ?? false);
        await FlutterBluetoothSerial.instance.cancelDiscovery();
        if (!await _connectToDevice(result.device)) {
          return false;
        }
      } on TimeoutException {
        logger.e('Timeout while searching for bluetooth device');
        return false;
      } on Exception {
        logger.e('Error while searching for bluetooth device');
        return false;
      } catch(e) {
        logger.e('Error while searching for bluetooth device: $e');
        return false;
      }
    }
    _listenPacket();
    return true;
  }

  void _listenPacket() {
    final BytesBuilder builder = BytesBuilder(copy: false);
    _connection!.input!.listen((event) {
      builder.add(event.takeWhile((value) => value != 0xFF).toList());
      if (builder.length == 5) {
        final packet = Packet.fromBytes(builder.takeBytes());
        builder.clear();
        _buffer.add(packet);
        logger.i('Received packet: ${packet.flag}, with data: ${packet.data.toHexString()}');
      } else if (builder.length > 5) {
        builder.clear();
      }
    });
  }

  Future<void> _getPreviousDevice() async {
    final prefs = await SharedPreferences.getInstance();
    _name = prefs.getString("bluetooth_name");
    _address = prefs.getString("bluetooth_address");
  }

  Future<bool> _connectToPreviousDevice() async {
    try {
      await _bondDeviceIfNeeded(_address!);
      _connection = await BluetoothConnection.toAddress(_address!);
    } catch(e) {
      logger.e(e);
      return false;
    }
    return true;
  }
  Future<bool> _connectToDevice(BluetoothDevice device) async {
    try {
      await _bondDeviceIfNeeded(device.address);
      _connection = await BluetoothConnection.toAddress(device.address);
      _address = device.address;
      _name = device.name;
      final prefs = await SharedPreferences.getInstance();
      prefs.setString("bluetooth_name", _name!);
      prefs.setString("bluetooth_address", _address!);
    } catch(e) {
      logger.e(e);
      _address = null;
      _name = null;
      return false;
    }
    return true;
  }

  Future<void> _bondDeviceIfNeeded(String address) async {
    if ((await FlutterBluetoothSerial.instance.getBondStateForAddress(address)) != BluetoothBondState.bonded) {
      await FlutterBluetoothSerial.instance.bondDeviceAtAddress(address, pin: "1324");
    }
  }

  String get name => _connection?.isConnected == true ? _name! : "Aucun poulailler detecté";
  bool get isConnected => _connection?.isConnected ?? false;
  Future<void> disconnect() async {
    _buffer = [];
    await _connection?.close();
    _connection?.dispose();
  }

  Future<dynamic>? sendPacket(Flags flag, [Uint8List? data]) {
    final packet = Uint8List(5);
    packet[0] = flag.index;
    packet[1] = data?[0] ?? 0;
    packet[2] = data?[1] ?? 0;
    packet[3] = data?[2] ?? 0;
    packet[4] = data?[3] ?? 0;
    logger.i('Sending packet: $flag, with data: ${data?.toHexString() ?? [0, 0, 0, 0]}');
    try {
      _connection?.output.add(packet);
      return _connection?.output.allSent;
    } catch(e) {
      logger.e(e);
      disconnect();
      return null;
    }
  }
  Future<Packet?> readPacket(Flags wantedFlag, [Uint8List? data]) {

    return _findBufferVal(wantedFlag, data);
  }

  Future<int> getTemperature() async {
    await sendPacket(Flags.getCurrentTemp);
    final packet = await readPacket(Flags.getCurrentTemp);
    if (packet == null) {
      return getTemperature();
    }
    return packet.data.buffer.asByteData().getInt32(0, Endian.little);
  }

  Future<bool> getDoorState() async {
    await sendPacket(Flags.getDoorStatus);
    final packet = await readPacket(Flags.getDoorStatus);
    if (packet == null) {
      return getDoorState();
    }
    return packet.data.buffer.asByteData().getInt32(0, Endian.little) == 1;
  }

  Future<bool> getDoorFree() async {
    await sendPacket(Flags.getDoorFree);
    final packet = await readPacket(Flags.getDoorFree);
    if (packet == null) {
      return getDoorFree();
    } 
    return packet.data.buffer.asByteData().getInt32(0, Endian.little) == 1;
  }

  closeDoor() async {
    await sendPacket(Flags.closeDoor);
    await readPacket(Flags.closeDoor);
  }
  openDoor() async {
    await sendPacket(Flags.openDoor);
    await readPacket(Flags.openDoor);
  }
  freeDoor() async {
    await sendPacket(Flags.freeDoor);
    await readPacket(Flags.freeDoor);
  }

  Future<DateTime?> getCurrentTime() async {
    await sendPacket(Flags.getCurrentTime);
    final packet = await readPacket(Flags.getCurrentTime);
    if (packet == null) {
      return null;
    }
    return DateTime.fromMillisecondsSinceEpoch(packet.data.buffer.asByteData().getUint32(0, Endian.little) * 1000);
  }

  Future<void> setCurrentTime() async {
    int epoch = (DateTime.now().millisecondsSinceEpoch / 1000).round().toUnsigned(32);
    final data = Uint8List.fromList([epoch & 0xFF, (epoch >> 8) & 0xFF, (epoch >> 16) & 0xFF, (epoch >> 24) & 0xFF]);
    logger.d(data.toHexString());
    logger.d(data.buffer.asByteData().getUint32(0, Endian.little));
    await sendPacket(Flags.setCurrentTime, data);
  }

  Future<DateTime> getSunsetTime() async {
    await sendPacket(Flags.getSunsetTime);
    final packet = await readPacket(Flags.getSunsetTime);
    if (packet == null) {
      return getSunsetTime();
    }
    return DateTime.fromMillisecondsSinceEpoch(packet.data.buffer.asByteData().getUint32(0, Endian.little));
  }

  Future<DateTime> getSunriseTime() async {
    await sendPacket(Flags.getSunriseTime);
    final packet = await readPacket(Flags.getSunriseTime);
    if (packet == null) {
      return getSunriseTime();
    }
    return DateTime.fromMillisecondsSinceEpoch(packet.data.buffer.asByteData().getUint32(0, Endian.little));
  }

  Future<Packet?> _findBufferVal(Flags flag, Uint8List? data) {
    return Future(() async {
      bool timedout = false;
      final timer = Timer(const Duration(seconds: 2), () {
        logger.e("Timeout for $flag, with data: ${data?.toHexString() ?? [0, 0, 0, 0]}");
        timedout = true;
      });
      int packetIndex;
      do {
        if (timedout) {
          return null;
        }
        // Find with the flag of the element and if there is data to compare
        packetIndex = _buffer.indexWhere((element) => element.flag == flag && (data == null || listEquals(data, element.data)));
        if (packetIndex == -1) {
          await Future.delayed(const Duration(milliseconds: 50));
        }
      } while(packetIndex == -1 || _buffer.isEmpty);
      timer.cancel();
      return _buffer.removeAt(packetIndex);
    });
    
  }
}

class Packet {
  final Flags flag;
  final Uint8List data;

  Packet(this.flag, this.data);

  static Packet fromBytes(Uint8List bytes) {
    return Packet(Flags.values[bytes[0]], bytes.sublist(1));
  }

  int toInt() {
    return data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3];
  }
}