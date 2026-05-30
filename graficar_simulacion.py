import matplotlib.pyplot as plt
import pandas as pd

# 1. Cargar los datos desde el archivo generado por C++
# Se utiliza '\t' como separador ya que los datos se exportaron con tabulaciones
try:
    df = pd.read_csv('C:/Users/Andres/OneDrive - Universidad de Antioquia/Escritorio/INFORMATICA_II/Lab_5/simulacion_datos.txt', sep='\t')
except FileNotFoundError:
    print("Error: No se encontró el archivo 'simulacion_datos.txt'. Asegúrate de ejecutar primero la simulación en Qt.")
    exit()

# 2. Filtrar los eventos de movimiento continuo para trazar las líneas de trayectoria
df_movimiento = df[df['Evento'] == 'Movimiento']

# 3. Filtrar los eventos de colisión para marcarlos con puntos específicos
df_colisiones = df[df['Evento'].str.contains('Colision')]

# Configurar el lienzo de la gráfica
plt.figure(figsize=(10, 6))

# 4. Agrupar los datos por ID de partícula y graficar cada trayectoria
for particle_id, group in df_movimiento.groupby('ID'):
    # Convertir explícitamente a variables numéricas (float)
    x = pd.to_numeric(group['Pos_X'])
    y = pd.to_numeric(group['Pos_Y'])
    plt.plot(x, y, label=f'Partícula {particle_id}')

# 5. Marcar los puntos exactos donde ocurrieron colisiones
if not df_colisiones.empty:
    x_col = pd.to_numeric(df_colisiones['Pos_X'], errors='coerce')
    y_col = pd.to_numeric(df_colisiones['Pos_Y'], errors='coerce')
    # zorder=5 asegura que las "X" rojas se dibujen por encima de las líneas
    plt.scatter(x_col, y_col, color='red', marker='x', s=100, label='Colisiones', zorder=5)

# 6. Configuración visual y geométrica
plt.title('Trayectorias de la Simulación de Partículas')
plt.xlabel('Posición X')
plt.ylabel('Posición Y')

# Invertir el eje Y: El sistema de coordenadas de QGraphicsScene (Qt) tiene el (0,0) arriba a la izquierda.
# Matplotlib lo tiene abajo a la izquierda. Invertirlo asegura que la gráfica coincida con lo que viste en la interfaz.
plt.gca().invert_yaxis() 

plt.legend()
plt.grid(True)
plt.axis('equal') # Fuerza a que la escala en X y Y sea la misma para que los círculos no se vean elípticos

# Renderizar la gráfica
plt.show()