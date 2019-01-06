
from bs4 import BeautifulSoup as bs
import urllib
import re
import os

def scrape(baseurl, suffix, entity):
    URL = baseurl+suffix
    soup = bs(urllib.request.urlopen(urllib.request.Request(URL, headers={'User-Agent': 'Mozilla/5.0'})))
    links = soup.find_all('a')

    f = open('%s.txt' %entity, 'a+')
    g = open('links_%s.txt' %entity, 'a+')

    for link in links:
        if link.string is not None:
            f.write(link.string.strip() + '\n')
            if link['href'].startswith('http'):
                g.write(link['href'] + '\n')
            else:
                g.write(baseurl + link['href'] + '\n')

    f.close()
    g.close()

if __name__=='__main__':
    scrape(baseurl = 'https://www.phrases.org.uk/meanings/', suffix='phrases-and-sayings-list.html/', entity = 'phrases')
    scrape(baseurl = 'https://www.phrases.org.uk/', suffix='idioms/a-z/full-list.html', entity = 'idioms')
    scrape(baseurl = 'https://www.phrases.org.uk/meanings/', suffix= 'proverbs.html/', entity = 'proverbs')
