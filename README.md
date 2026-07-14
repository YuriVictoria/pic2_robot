
# Robô Aspirador Autônomo (ROS2 & RPLIDAR)

[![ROS2](https://img.shields.io/badge/ROS2-Jazzy-brightgreen.svg)](https://docs.ros.org/en/jazzy/)
[![Ubuntu](https://img.shields.io/badge/Ubuntu-24.04-orange.svg)](https://ubuntu.com/)
[![Arduino](https://img.shields.io/badge/Arduino-UNO-blue.svg)](https://www.arduino.cc/)
[![RPLIDAR](https://img.shields.io/badge/RPLIDAR-purple.svg)](https://slamtec.com/en/lidar/a2/)

**Repositório Público:** [Github](https://github.com/YuriVictoria/pic2_robot).

**Repositório do projeto base:** [Github](https://github.com/dinishsk/autonomous-mobile-vacuum-cleaning-robot).

**Equipe:** [Edgard](https://github.com/Edluck), [Felipe Carpes](https://github.com/carpesfelipe), [Klarine Silva](https://github.com/klarinemend) e [Yuri Victoria](https://github.com/YuriVictoria).

## 1. Introdução e definição

Este projeto consiste no desenvolvimento de um robô aspirador autônomo, projetado para realizar o mapeamento de ambientes em tempo real e a limpeza deste ambiente, utilizando um algoritmo ótimo de varredura para melhor eficiência da limpeza.

A montagem física do equipamento foi em uma carcaça de vassoura, com duas cerdas, e uma caixa preta que oculta os circuitos e suporta o sensor Lidar. A arquitetura do chassi é dividida em três andares:

* **Primeiro Andar:** Parte interna inferior. Onde está o motor que gira as cerdas(para frente ou para trás, a depender do sentido do robô), dois motores para as rodas, encoders magnéticos e seus apoios nas rodas, e um compartimento interno, onde a sujeira se armazena.
* **Segundo Andar:** É uma caixa preta, a fim de esconder o circuito, que possui toda a alimentação e processamento do sistema, pois onde estão os microcontroladores, baterias e ponte H dos motores.
* **Terceiro Andar:** Posicionado no topo do chassi. Abriga exclusivamente o sensor LiDAR, de forma a garantir o campo de visão de 360 graus.

<img width="836" height="631" alt="imagem 1 robo" src="https://github.com/user-attachments/assets/cb0a7a58-1eab-42d3-86ce-d1079b0c754a" />


---

## 2. Arquitetura de Hardware e Software

### 2.1. Hardware

#### 2.1.1. Lista de Equipamentos e Componentes
Abaixo estão os dispositivos eletrônicos utilizados:
* **Microcontroladores e Processamento:**
  * 1x Microcomputador Raspberry Pi 4B.
  * 1x Microcontrolador Arduino UNO R3.
* **Sensores e Reconhecimento:**
  * 1x Sensor LiDAR 2D RPLIDAR A2M12.
  * 2x Encoders Magnéticos Absolutos AS5600.
* **Atuadores e Potência:**
  * 2x Motores CC 3-6v para tração com rodas de 68mm.
  * 1x Motor de sucção (adaptado de aspirador portátil de 12V).
  * 2x Drivers de motor tipo Ponte H L298N.
* **Alimentação:**
  * 1x Powerbank de 12V, 12200mAh (Para o Raspberry, Arduíno e RPLIDAR).
  * 1x Bateria LiPo 7.6V (Para os motores).
  * 1x Bateria 9V (para o motor das cerdas).

#### 2.1.2. Modelagem 3D Estrutural
O chassi e adaptações estruturais foram projetados e modelados pela equipe para acomodar os componentes descritos.

Modelo 3D do robô:

<img width="422" height="324" alt="imagem 2 modelo3d" src="https://github.com/user-attachments/assets/5d34b263-1708-4de2-a044-2e6736a6fe9a" />

Suportes utilizados para os encoders AS5600:

<img width="1920" height="858" alt="Imagem 3 suporteEncoder" src="https://github.com/user-attachments/assets/46d38164-ff3e-4d11-9eea-f5dda80c58ff" />

Arquivo de Impressão stl: suporteAS5600_impressao.stl


Suportes com o motor:

<img width="1920" height="858" alt="imagem 4 suporteComMotor" src="https://github.com/user-attachments/assets/addf712e-41e4-43d5-83bb-ec66b68d1fcc" />



O trabalho utilizou engrenagens para girar as cerdas do chassi da vassoura, a engrenagem acoplada no motor 3-6V, faz girar a engrenagem junta a engrenagem das cerdas(3 cerdas que giram pela mesma engrenagem do próprio chassi)

Arquivo de impressão: engrenagens-38-39-40_impressao.stl

#### 2.1.3. Projeto Eletrônico e Placas

<img width="939" height="697" alt="Imagem 5 fluxograma" src="https://github.com/user-attachments/assets/74c66dff-a6d2-44a6-925d-6e5c5c94144b" />


Este é o projeto deste trabalho, como a succção não foi possível realizar, ela não consta neste projeto, mas basta adicionar um relé do arduíno para o aspirador portátil.

### 2.2. Software

O código-fonte do projeto está dividido entre a lógica embarcada nos atuadores e o software de interface e processamento espacial, devidamente associados aos seus hardwares de destino.

* **Código Embarcado (Destino: Arduino UNO R3):**
  * Responsável pela leitura dos encoders AS5600 e pelo controle em malha fechada via PWM das Pontes H.
  * [Localização no repositório](https://github.com/YuriVictoria/pic2_robot/tree/main/arduino)
* **Software de Interface e Controle (Destino: Raspberry Pi 4B):**
  * Desenvolvido sob o ROS2 Jazzy. Contém os nós responsáveis por processar a odometria, ler as mensagens do LiDAR via serial, executar o algoritmo SLAM e planejar a navegação.
  * [Localização no repositório](https://github.com/YuriVictoria/pic2_robot/tree/main/pic2)
  * [Ubuntu Server](https://ubuntu.com/download/server)

---

## 3. Como funciona

O funcionamento do sistema é feito pelo ROS2 Jazzy em Ubuntu Server 24.04, coordenando o hardware através de comunicação serial estruturada no seguinte fluxo de operação:

1.  **Controle do Arduino:** O microcontrolador processa as leituras angulares fornecidas pelos encoders AS5600 nas duas rodas. Simultaneamente, recebe os vetores de velocidade do Raspberry Pi e os converte em sinais PWM para as Pontes H, executando o controle dos motores das rodas e das Cerdas.
2.  **Controle do robô:** O pacote `diff-drive-controller` atua como a ponte de software. Ele traduz as métricas dos encoders em dados de odometria, calculando o deslocamento físico do robô no plano, e publica as transformadas espaciais (TF). 
3.  **slam_toolbox:** O algoritmo junta a odometria com a nuvem de pontos lida pelo RPLIDAR. Este processo gera um mapa bidimensional contínuo (grade de ocupação) e localiza o equipamento dentro da sala em tempo real.
4.  **Nav2:** Utilizando o mapa gerado, o sistema Nav2 computa a trajetória ideal para a cobertura do piso, desviando de obstáculos de forma autônoma.

A comunicação é feita pela publicação dos dados e consumo desses dados por tópicos, o ROS2 administra essas informações para fazer o controle desejado. Com relação a navegação, basicamente no algoritmo dizemos que obstáculos possuem custo alto e o chão custo baixo, logo a trajetória a ser calculada é para evitar colisão com os obstáculos. Os código estão organizados no repositório do github.

<img width="1600" height="1200" alt="Imagem 6 visaoInternaRobo" src="https://github.com/user-attachments/assets/567cf744-f28a-4de8-8776-3bf086747fef" />



**Vídeo do funcionamento do Robô:**
[Vídeo do Robô](https://youtu.be/-LY7q7LwVJo)

---

## 4. Como Usar

Esta seção detalha os procedimentos de instalação, compilação e inicialização do sistema.

### 4.1. Instalação de Dependências
No terminal, instale os controladores vitais do ROS2 executando:
```bash
sudo apt update
sudo apt install ros-jazzy-ros2-control ros-jazzy-ros2-controllers ros-jazzy-robot-state-publisher ros-jazzy-joint-state-publisher ros-jazzy-joint-state-publisher-gui ros-jazzy-joint-state-broadcaster ros-jazzy-diff-drive-controller libserial-dev
```
### 4.2 Compilação do Workspace
No terminal, acesse a raiz do seu workspace (ex: ros2_ws) e execute a compilação. A flag --symlink-install é recomendada para permitir a edição de scripts e arquivos sem a necessidade de recompilação:
```bash
colcon build --symlink-install
```
```bash
source install/setup.bash
```
Nota: Para solução de problemas de compilação, limpe o cache excluindo os diretórios: rm -rf build/ install/ log/ e recompile.
### 4.3 Configuração das Portas USB
Para evitar conflitos de permissão, configure as regras estáticas do Udev. Certifique-se de ter carregado o ambiente (source install/setup.bash).

Para fixar o LiDAR em /dev/rplidar:
```bash
cd src/Packages/rplidar_ros/scripts/
chmod +x create_udev_rules.sh
./create_udev_rules.sh
```
Para fixar o microcontrolador em /dev/arduino:
```bash
cd ../../diffdrive_arduino/scripts/
chmod +x create_udev_rules.sh
./create_udev_rules.sh
```
Desconecte e reconecte os cabos USB. Para verificar, utilize: ls -l /dev | grep -E "rplidar|meu_arduino".
### 4.4 Execução do Sistema
Caso ocorram erros de dependência, execute na raiz do workspace:
```bash
rosdep install --from-paths src --ignore-src -r -y
```
Abra diferentes terminais, carregue o ambiente em todos (source install/setup.bash) e execute os módulos na seguinte ordem.
Nota: Para testes com o hardware físico (sem simulação), certifique-se de utilizar o argumento use_sim_time:=False após cada comando de launch.

Para iniciar a comunicação com o hardware e a leitura do LiDAR:
```bash
ros2 launch diffdrive_arduino diffbot.launch.py use_sim_time:=False
```
Para inicializar o algoritmo de Mapeamento (SLAM):
```bash
ros2 launch slam_toolbox online_async_launch.py use_sim_time:=False
```
Para piloto automático e navegação:
```bash
ros2 launch nav2_bringup navigation_launch.py use_sim_time:=False
```
Para operação manual (Teleoperação):
```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -p stamped:=true -p frame_id:=base_link
```
