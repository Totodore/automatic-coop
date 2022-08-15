import 'dart:async';

import 'dart:typed_data';

Future waitWhile(bool test(), [Duration pollInterval = Duration.zero]) {
  var completer = Completer();
  check() {
    if (!test()) {
      completer.complete();
    } else {
      Timer(pollInterval, check);
    }
  }
  check();
  return completer.future;
}