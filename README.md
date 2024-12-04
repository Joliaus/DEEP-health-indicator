# Indicateur de santé par mesure cardiaque et thermique

Ce projet consiste à développer un système portable permettant de mesurer l'état de santé d'un utilisateur en temps réel. Le dispositif utilise des capteurs pour surveiller la fréquence cardiaque (BPM) et la température corporelle, et affiche les résultats sur un écran TFT tactile. En cas de valeurs anormales, des alertes visuelles et Bluetooth sont générées.

## Fonctionnalités

- **Mesure en temps réel** :
  - Fréquence cardiaque (BPM) via un capteur de pouls.
  - Température corporelle via un capteur infrarouge.
- **Affichage des données** :
  - Utilisation d'un écran TFT tactile pour afficher les constantes.
- **Alertes automatiques** :
  - Alerte visuelle avec une matrice LED (cœur battant).
  - Alerte via Bluetooth sur un smartphone.
- **Compact et portable** :
  - Boîtier ergonomique adapté pour un doigt.

## Architecture matérielle

- **Écran TFT** : ILI9341 (240x320) avec contrôleur XPT2046.
- **Capteur de température** : MLX90614.
- **Capteur de pouls** : Photodiode avec signal amplifié.
- **Matrice LED** : 8x8 LEDs adressables (WS2812).
- **Module Bluetooth** : HC-05 pour la communication sans fil.

## Prérequis

- **Microcontrôleur** : STM32F103 (BluePill).
- **Logiciels nécessaires** :
  - STM32CubeIDE : pour le développement du firmware.
  - Altium Designer : pour le design de PCB (optionnel).
  - Fusion 360 : pour la modélisation 3D du boîtier.

## Installation et Compilation

1. **Cloner le dépôt** :
   ```bash
   git clone https://github.com/Joliaus/DEEP-health-indicator.git
   cd votre-repo
