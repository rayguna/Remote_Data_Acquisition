from epics import caget

#get TDS value
caget(“uno:tds”)

#get water level value
caget(“uno:wl”)

#get temperature value
caget(“uno:dhtt”)

#get humidity value
caget(“uno:dhth”)
