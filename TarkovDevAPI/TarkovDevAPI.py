import sqlite3
import requests

def run_query(query):
    headers = {"Content-Type": "application/json"}
    response = requests.post('https://api.tarkov.dev/graphql', headers=headers, json={'query': query})
    if response.status_code == 200:
        return response.json()
    else:
        raise Exception("Query failed to run by returning code of {}. {}".format(response.status_code, query))

new_query = """
{
  items(lang: en) {
    id
    sellFor {
      priceRUB
      vendor {
        name
      }
    }
    shortName
  }
}
"""

result = run_query(new_query)

ResultSize = len(result['data']['items'])

con = sqlite3.connect('C:\\Users\\admin\\source\\repos\\sqlite_testing\\EFT_Data.db')
cur = con.cursor()

for i in range(ResultSize):
    item = result['data']['items'][i]

    HighestPrice = -1;
    for sellOption in item['sellFor']:
        price = sellOption['priceRUB']
        vendor = sellOption['vendor']['name']
        if(vendor == "Flea Market"):          
            continue

        if price > HighestPrice:
            HighestPrice = price

    cur.execute("INSERT INTO item_data (bsg_id, short_name, trader_price) VALUES (?, ?, ?)", (item['id'], item['shortName'], HighestPrice))

con.commit()
con.close() 