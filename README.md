## Instalação

No terminal, faça as seguintes instações:

```bash
sudo apt install ros-jazzy-ros2-control
sudo apt install ros-jazzy-ros2-controllers
sudo apt install ros-jazzy-robot-state-publisher
sudo apt install ros-jazzy-joint-state-publisher
sudo apt install ros-jazzy-joint-state-publisher-gui
sudo apt install ros-jazzy-joint-state-broadcaster
sudo apt install ros-jazzy-diff-drive-controller
sudo apt install libserial-dev
```
---

## 🛠️ Compilando o Workspace

No terminal, certifique-se de estar na raiz do seu workspace (`ros2_ws`) e execute o comando de compilação:

```bash
colcon build --symlink-install

```

*(**Dica:** O argumento `--symlink-install` cria atalhos para os seus scripts Python e arquivos de launch. Isso significa que você pode editar esses arquivos e testá-los na hora, sem precisar recompilar o projeto!)*

Após a compilação terminar com sucesso, é **obrigatório** carregar o ambiente do seu workspace no terminal para que o ROS 2 reconheça os seus pacotes:

```bash
source install/setup.bash

```

### 🧹 Limpando o Workspace (Troubleshooting)

O ROS 2 guarda arquivos de cache para acelerar a compilação. Se você deletar pacotes, alterar arquivos estruturais (como `CMakeLists.txt` ou `package.xml`) ou enfrentar erros inexplicáveis ao compilar, a melhor solução é fazer uma **instalação limpa**.

Para limpar o workspace, apague as pastas geradas pelo colcon e compile novamente:

```bash
# Apaga as pastas de compilação antigas
rm -rf build/ install/ log/

# Compila tudo do zero novamente
colcon build --symlink-install

```

---

## 🔌 Configuração das Portas USB (udev rules)

Para que o ROS 2 consiga se comunicar corretamente com o hardware sem problemas de permissão (acesso negado) e sem que as portas mudem de nome ao reconectar os cabos, precisamos configurar as regras do `udev` no Linux.

Isso garantirá que:

* O LiDAR seja sempre reconhecido como `/dev/rplidar`
* O Arduino seja sempre reconhecido como `/dev/arduino`

### ⚠️ Pré-requisito

Antes de executar os scripts, certifique-se de que você já compilou o workspace (`colcon build`) e que carregou o ambiente do ROS, pois os scripts utilizam ferramentas do ROS para localizar as pastas.

No terminal, na raiz do seu workspace (`ros2_ws`), execute:

```bash
source install/setup.bash

```

*(Nota: Para o Arduino, certifique-se de que você já editou o arquivo `arduino.rules` com os IDs corretos da sua placa obtidos via comando `lsusb`).*

---

### 1️⃣ Configurando o RPLiDAR

Navegue até a pasta de scripts do pacote do LiDAR e execute o script de criação:

```bash
cd src/Packages/rplidar_ros/scripts/
chmod +x create_udev_rules.sh
./create_udev_rules.sh

```

### 2️⃣ Configurando o Arduino

Agora, volte para a pasta `src` e navegue até os scripts do seu pacote do Arduino:

```bash
cd ../diffdrive_arduino/scripts/
chmod +x create_udev_rules.sh
./create_udev_rules.sh

```

### ✅ Verificando a Instalação

Para garantir que tudo funcionou:

1. Desconecte e conecte novamente os cabos USB do Arduino e do RPLiDAR no computador.
2. Execute o comando abaixo no terminal para listar os dispositivos conectados:

```bash
ls -l /dev | grep -E "rplidar|arduino"

```

Se a configuração foi bem-sucedida, você verá `/dev/rplidar` e `/dev/arduino` apontando para as portas reais (como `ttyUSB0` ou `ttyACM0`), com permissões totais (`lrwxrwxrwx`).

---

**Desinstalação:**
Caso precise remover essas regras no futuro, basta navegar até as mesmas pastas citadas acima e executar o script `./delete_udev_rules.sh`.

---

## Usage

For Launching the LiDAR:
```bash
ros2 launch rplidar_ros rplidar_a2m12_launch.py
```
For Connecting the RaspberryPi and Arduino:
```bash
ros2 launch diffdrive_arduino diffbot.launch.py
```
For Teleoperation (Original = ERRO):
```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r /cmd_vel:=/diffbot_base_controller/cmd_vel_unstamped
```
For Teleoperation (Use this):
```bash
ros2 run teleop_twist_keyboard teleop_twist_keyboard --ros-args -r cmd_vel:=/diffbot_base_controller/cmd_vel -p stamped:=true -p frame_id:=base_link
```
For Navigation:
```bash
ros2 launch nav2_bringup navigation_launch.py
```
For Launching the SLAM:
```bash
ros2 launch slam_toolbox online_async_launch.py
```
Incase of any errors, Install the dependencies, for that go to the workspace folder and execute the following command:
```bash
rosdep install --from-paths src --ignore-src -r -y
```