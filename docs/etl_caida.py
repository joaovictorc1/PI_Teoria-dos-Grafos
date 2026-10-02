import pandas as pd
import math
import random

def haversine(lat1, lon1, lat2, lon2):
    R = 6371.0 # Raio da Terra em KM
    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)
    a = (math.sin(dlat / 2) * math.sin(dlat / 2) +
         math.cos(math.radians(lat1)) * math.cos(math.radians(lat2)) *
         math.sin(dlon / 2) * math.sin(dlon / 2))
    c = 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))
    return R * c

def processar_caida(caminho_geo, caminho_links, max_nos=1000):
    print("Iniciando ETL do CAIDA ITDK...")
    
    # 1. LER ARQUIVO DE GEOLOCALIZAÇÃO
    # Formato esperado: node_id, continent, country, region, city, lat, lon
    print("Lendo nós e filtrando pelo Brasil...")
    nos_br = {}
    with open(caminho_geo, 'r', encoding='utf-8') as f:
        for linha in f:
            if linha.startswith('#'): continue
            partes = linha.strip().split('\t')
            if len(partes) >= 7 and partes[2] == 'BR': # Filtra Brasil
                node_id = partes[0]
                lat = float(partes[5])
                lon = float(partes[6])
                nos_br[node_id] = (lat, lon)
                
                if len(nos_br) >= max_nos:
                    break

    # 2. LER ARQUIVO DE LINKS (Topologia)
    # Formato esperado: link_id, node1, node2, ...
    print("Processando arestas (links)...")
    arestas = []
    nos_conectados = set()
    
    with open(caminho_links, 'r', encoding='utf-8') as f:
        for linha in f:
            if linha.startswith('#'): continue
            partes = linha.strip().split()
            # O CAIDA links normalmente tem um node inicial e vários adjacentes (node1:link node2:link)
            # Adaptamos para ler as conexões. Exemplo simplificado (origem, destino):
            if len(partes) >= 3:
                # Remove prefixos como 'N' se houver
                n1 = partes[1].split(':')[0]
                n2 = partes[2].split(':')[0]
                
                if n1 in nos_br and n2 in nos_br:
                    lat1, lon1 = nos_br[n1]
                    lat2, lon2 = nos_br[n2]
                    distancia = haversine(lat1, lon1, lat2, lon2)
                    
                    arestas.append({'origem': n1, 'destino': n2, 'peso_km': round(distancia, 2)})
                    nos_conectados.add(n1)
                    nos_conectados.add(n2)
                    
            if len(nos_conectados) >= max_nos:
                break

    # 3. EXPORTAR PARA CSV PARA O PROGRAMA EM C
    print(f"Exportando {len(nos_conectados)} vértices e {len(arestas)} arestas...")
    
    with open('vertices_br.csv', 'w') as f_v:
        f_v.write("id_interno,id_caida,lat,lon\n")
        mapa_ids = {} # O C prefere IDs sequenciais de 0 a N-1
        for i, no_id in enumerate(nos_conectados):
            mapa_ids[no_id] = i
            lat, lon = nos_br[no_id]
            f_v.write(f"{i},{no_id},{lat},{lon}\n")
            
    with open('arestas_br.csv', 'w') as f_a:
        f_a.write("origem,destino,peso\n")
        for a in arestas:
            o_idx = mapa_ids[a['origem']]
            d_idx = mapa_ids[a['destino']]
            f_a.write(f"{o_idx},{d_idx},{a['peso_km']}\n")

    print("ETL Concluído. Arquivos 'vertices_br.csv' e 'arestas_br.csv' gerados.")

if __name__ == '__main__':
    # Você deverá baixar os arquivos do CAIDA ITDK e apontar os caminhos reais aqui.
    print("Aponte para os arquivos .geo e .links do CAIDA para executar.")
    # processar_caida('caminho/para/itdk.geo', 'caminho/para/itdk.links')
