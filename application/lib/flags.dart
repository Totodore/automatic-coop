
// Packet Size: [Flag(1)] [Data(4)]
enum Flags
{
  ack,
  getDoorStatus,
  getCurrentTime,
  getSunsetTime,
  getSunriseTime,
  getCurrentTemp,
  closeDoor,
  openDoor,
  freeDoor,
  setCurrentTime,
  getDoorFree,
}