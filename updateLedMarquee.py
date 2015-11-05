#!/usr/bin/env python
from bs4 import BeautifulSoup
import urllib2
import datetime
import socket
import requests
import json
import sys

# get the current time
now = datetime.datetime.now()

def getCinemaString():
	# whether to include showtimes in the morning or not
	INCLUDE_AM = False

	class Movie:
		def __init__(self, titleName, times):
			self.title = titleName
			self.showTimes = times

	rawHtmlPage = urllib2.urlopen("http://www.exhibitorads.com/csentry/a1website_relay.asp?house_id=27341&source=BOWTIEMOVIES&link=http://www.bowtiecinemas.com/movies/").read()

	soup = BeautifulSoup(rawHtmlPage)
	#c = soup.find('span', {'class':'vi'}).contents
	#print(soup.prettify())

	wholeTable = soup.find('table', {'class':'a1table'})
	#print(wholeTable.prettify())
	allRows = soup.find_all('tr')

	movies = []
	for row in allRows:
		rawTitle = row.find('a', {'class':'a1title'})
		title = ''
		if(rawTitle):
			title = rawTitle.b.string
			#print title

			timesBlock = row.find('span', {'class':'a1times'})
			showTimes = []
			if(timesBlock):
				rawTimes = timesBlock.find_all('a')
				for rawTime in rawTimes:
					if(rawTime):
						timeString = rawTime.string
						# get rid of parens
						timeString = timeString.replace('(',"")
						timeString = timeString.replace(')',"")

						# We usually don't care about morning shows
						if( ('AM' not in timeString) or INCLUDE_AM):
							hour = int(timeString.split(':')[0])
							# compensate for the whole 24 hour madness
							if( hour!=12 ):
								hour+=12

							mins = timeString.split(':')[1]
							indexOfPM = mins.find('PM')
							if indexOfPM > 0:
								mins = mins[0:indexOfPM]

							showTime = datetime.datetime.now()
							showTime = showTime.replace(hour=int(int(hour)), minute=int(mins), second=0, microsecond=0)
							
							if(now < showTime):
								showTimes.append(showTime)

			movies.append( Movie(title, showTimes) )

	# compile list of all movies that still have showtimes today (get rid of past times)
	moviesToDisplay = [ movie for movie in movies if movie.showTimes ]

	#print "OK, here are the movies we will actually display"
	displayStr = ''
	for j, movie in enumerate(moviesToDisplay):
		if(j is not 0):
			displayStr+=', '   #ending comma	
		#print movie.title
		displayStr += movie.title + " ("

		for i, time in enumerate(movie.showTimes):
			#print "\t" + str(time)
			# the first time through, we don't add a comma
			if(i is not 0):
				displayStr += ', '

			timestr = time.strftime("%I:%M")
			# get rid of leading zero, if it exists
			if( timestr[0] == '0'):
				displayStr += timestr[1:]
			else:
				displayStr += timestr

		displayStr += ')'
	
	return displayStr

def kelvinToFahr(tempk): 
	return (tempk * 9/5 - 459.67) 

def getWeather(city):
	weatherUgrndApiKey = "294eeb615c2db00471860d1c53b41902"
	weatherUgrndApiUrl = "http://api.openweathermap.org/data/2.5/weather?q="+city+"&APPID="+weatherUgrndApiKey
	r = requests.get(weatherUgrndApiUrl)
	try:
		response = json.loads(r.text)

		curTemp = kelvinToFahr( float(response['main']['temp']) )
		maxTemp = kelvinToFahr( float(response['main']['temp_max']) )
		minTemp = kelvinToFahr( float(response['main']['temp_min']) )
		humidity = response['main']['humidity']

		displayStr = str(int(curTemp))+'`, '+'H:'+str(int(maxTemp))+'` L:'+str(int(minTemp))+"`"
	except:
		print "Whoa Snap! Something went wrong. Returning empty weather string\n"
		print sys.exc_info()[0]
		displayStr = ""
	return displayStr

def getTime():
	timestr = now.strftime("%I:%M %p, %m/%d/%y")
	# get rid of leading zero, if it exists
	if( timestr[0] == '0'):
		timestr= timestr[1:]
	return timestr


#####################  MAIN  ######################
try:
	dispStr = getTime()+"  "+"  "+getWeather("reston")+"  "+getCinemaString()
except:
	print "Something bad happened"
	dispStr = "Something bad happened"

print "DisplayString: \n " + dispStr
clientsocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
clientsocket.connect(('192.168.1.12', 1336))
clientsocket.send(dispStr)

# Add this in so I can tell if the cron job actually ran.
#file = open("actuallyRan", 'a')
#file.write("Ran this file at: " + now.strftime("%I:%M %p") + " on " + now.strftime("%m/%d/%Y") + "\n")
#file.close()