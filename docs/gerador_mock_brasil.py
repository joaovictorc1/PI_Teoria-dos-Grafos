import random
import math

def haversine(lat1, lon1, lat2, lon2):
    R = 6371.0
    dlat = math.radians(lat2 - lat1)
    dlon = math.radians(lon2 - lon1)
    a = math.sin(dlat/2)**2 + math.cos(math.radians(lat1))*math.cos(math.radians(lat2))*math.sin(dlon/2)**2
    return R * 2 * math.atan2(math.sqrt(a), math.sqrt(1 - a))

def gerar_dados_mock(num_vertices=1000):
    print(f"Gerando dados sintéticos para {num_vertices} roteadores no Brasil...")
    
    # Bounding box aproximado do Brasil
    lat_min, lat_max = -33.7, 5.2
    lon_min, lon_max = -73.9, -34.7
    
    vertices = []
    with open('vertices_br.csv', 'w') as f_v:
        f_v.write("id,lat,lon\n")
        for i in range(num_vertices):
            lat = random.uniform(lat_min, lat_max)
            lon = random.uniform(lon_min, lon_max)
            vertices.append((i, lat, lon))
            f_v.write(f"{i},{lat:.4f},{lon:.4f}\n")
            
    # Criar arestas (Garantindo conectividade básica e algumas pontes)
    print("Gerando arestas...")
    with open('arestas_br.csv', 'w') as f_a:
        f_a.write("origem,destino,peso\n")
        
        # Conecta o i com i+1 para garantir que é pelo menos uma linha conexa (simplificação)
        for i in range(num_vertices - 1):
            peso = haversine(vertices[i][1], vertices[i][2], vertices[i+1][1], vertices[i+1][2])
            f_a.write(f"{i},{i+1},{peso:.2f}\n")
            
        # Adiciona arestas aleatórias (esparsidade de uma rede real)
        num_arestas_extras = num_vertices * 2
        for _ in range(num_arestas_extras):
            u = random.randint(0, num_vertices - 1)
            v = random.randint(0, num_vertices - 1)
            if u != v:
                peso = haversine(vertices[u][1], vertices[u][2], vertices[v][1], vertices[v][2])
                f_a.write(f"{u},{v},{peso:.2f}\n")

    print("Dados gerados com sucesso!")

if __name__ == '__main__':
    gerar_dados_mock(1000)
