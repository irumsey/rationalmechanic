import sys
import re
import http.client

HOST = 'horizons.jpl.nasa.gov'

URL = (
    "https://ssd.jpl.nasa.gov/api/horizons.api?format=text" + "&" +
    "COMMAND='399'" + "&" +
    "CENTER='500@10'" + "&" +
    "OBJ_DATA='YES'" + "&" +
    "MAKE_EPHEM='YES'" + "&" +
    "EPHEM_TYPE='ELEMENTS'" + "&" +
    "START_TIME='2000-01-01'" + "&" +
    "STOP_TIME='2050-01-01'" + "&" +
    "STEP_SIZE='1y'" + "&" +
    "ELEM_LABELS='NO'" + "&" +
    "QUANTITIES='A'" + "&" +
    "OUT_UNITS='AU-D" + "&" +
    "CSV_FORMAT='YES'" + "&" +
    "CAL_FORMAT='JD'"
)

connection = http.client.HTTPSConnection(HOST, timeout = 10)
connection.request("GET", URL)
response = connection.getresponse()

if 200 != response.status:
    print(response.reason)
    sys.exit(2)

decoded = response.read().decode("ascii")

print(decoded)

firstIndex = decoded.find("$$SOE") + 6
lastIndex = decoded.find("$$EOE") - 1

if not firstIndex < lastIndex:
    print("didn't work, boo")
    sys.exit(2)
    
body = decoded[firstIndex : lastIndex]
tokens = re.split(r'\s*,\s*', body)
del tokens[-1]

elements = [tokens[i : i + 14] for i in range(0, len(tokens), 14)]

with open('output.txt', 'w') as file:
    for element in elements:
        line = (
            "[ " +
            element[ 0] + ", " +
            element[ 2] + ", " +
            element[ 3] + ", " +
            element[ 4] + ", " +
            element[ 5] + ", " +
            element[ 6] + ", " +
            element[ 7] + ", " +
            element[ 8] + ", " +
            element[ 9] + ", " +
            element[10] + ", " +
            element[11] + ", " +
            element[12] + ", " +
            element[13] + " ],\n"
        )
        file.write(line)
        