import sys
import optparse
import re
import http.client
import urllib.parse

HOST = 'horizons.jpl.nasa.gov'

#
#
#
def extractElements(fileName, target, center, dateStart, dateStop, stepSize, units, refPlane):
    params = urllib.parse.urlencode({
        'format': 'text',
        'COMMAND': target,
        'CENTER': center,
        'MAKE_EPHEM': 'YES',
        'EPHEM_TYPE': 'ELEMENTS',
        'START_TIME': dateStart,
        'STOP_TIME': dateStop,
        'STEP_SIZE': stepSize,
        'QUANTITIES': 'A',
        'ELEM_LABELS': 'NO',
        'OBJ_DATA': 'NO',
        'REF_PLANE': 'ECLIPTIC',
        'OUT_UNITS': 'AU-D',
        'CSV_FORMAT': 'YES',
        'CAL_FORMAT': 'JD'
        })
    URL = f"https://ssd.jpl.nasa.gov/api/horizons.api?{params}"    
    
    print(URL)
    
    connection = http.client.HTTPSConnection(HOST, timeout = 10)
    connection.request("GET", URL)
    response = connection.getresponse()

    if 301 == response.status:
        print(response.getheader('Location'))
        quit()

    if 200 != response.status:
        print(response.reason)
        quit()
        
    decoded = response.read().decode("ascii")
    
    firstIndex = decoded.find("$$SOE") + 6
    lastIndex = decoded.find("$$EOE") - 1

    if not firstIndex < lastIndex:
        print("didn't work, boo")
        quit()
    
    body = decoded[firstIndex : lastIndex]
    tokens = re.split(r'\s*,\s*', body)
    del tokens[-1]

    elements = [tokens[i : i + 14] for i in range(0, len(tokens), 14)]

    with open(fileName, 'w') as file:
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

#
#
#            
def main():
    optionParser = optparse.OptionParser()
    
    optionParser.add_option('--target', dest = 'target', default = '<undefined>')
    optionParser.add_option('--center', dest = 'center', default = '<undefined>')
    optionParser.add_option( '--start', dest =  'start', default = '<undefined>')
    optionParser.add_option(  '--stop', dest =   'stop', default = '<undefined>')
    optionParser.add_option(  '--step', dest =   'step', default = '1mo')
    optionParser.add_option( '--units', dest =  'units', default = 'AU-D')
    optionParser.add_option( '--plane', dest =  'plane', default = 'ECLIPTIC')
    
    (opts, args) = optionParser.parse_args()
    
    if 1 != len(args):
        print("must specify output file")
        quit()
 
    if '<undefined>' == opts.target:
        print("must specify target frame using --target")
        quit()
        
    if '<undefined>' == opts.center:
        print("must specify center frame using --center")
        quit()
        
    if '<undefined>' == opts.start:
        print("must specify start date using --start")
        quit()
        
    if '<undefined>' == opts.stop:
        print("must specify stop date using --stop")
        quit()
        
    extractElements(args[0],
                    opts.target,
                    opts.center,
                    opts.start,
                    opts.stop,
                    opts.step,
                    opts.units,
                    opts.plane
                    )

#
#
#  
if __name__ == '__main__':
	main()
