import pandas as pd
import sys

def calcular_media_tempo_por_modo(nome_arquivo):
    try:
        df = pd.read_csv(nome_arquivo)

        if 'tempo' in df.columns and 'modo' in df.columns:
            medias = df.groupby('modo')['tempo'].mean().reset_index()
            print("\nMédia da coluna 'tempo' agrupada por 'modo':")
            print(medias)
        else:
            print("As colunas 'tempo' e/ou 'modo' não foram encontradas no arquivo.")

    except FileNotFoundError:
        print(f"Arquivo '{nome_arquivo}' não encontrado.")
    except Exception as e:
        print(f"Erro ao processar o arquivo: {e}")

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Uso: python script.py nome_do_arquivo.csv")
    else:
        nome_arquivo = sys.argv[1]
        calcular_media_tempo_por_modo(nome_arquivo)
