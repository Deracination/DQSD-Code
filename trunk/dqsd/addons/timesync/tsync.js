function synchit(sn){
  var synch = new ActiveXObject("TimeSynchronization.CTimeSynch");
  synch.ServerName=sn;
  synch.Synchronization=true;
  synch.GetTimeFromServer=true;

}