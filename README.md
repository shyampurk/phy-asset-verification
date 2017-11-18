# Automated Physical Asset Verification using BLE

This repository contains the hardware setup details and source code for an automated asset verification system based on the BLE (Bluetooth Low Energy)

## OVERVIEW

This project uses Mediatek Linkit ONE board as a BLE gateway and Cypress 4.1 BLE / Beacon device as the BLE node. The application server is hosted on Thingsboard and enables users to track their physical assets attached with the BLE node. The LinkIt-One board acts as the BLE gateway that aggregates all beacon signals from the BLE nodes. It detects the presence or absense of assets in a specified area and updates this information on a dashboard hosted by the Thingsboard server.

## INTRODUCTION

This system is assumed to be installed in a Factory space or any close office space where there are movable or portable assets which need to be monitored. The system has two parts

1. Asset Management Server - The application server that runs on Thingsboard and also hosts the dashboard for real-time presence monitoring of assets.

2. Hardware - BLE Gateway along with the BLE end nodes that are configured as a beacon. Mediatek LinkitONE is the gateway and Cypress 4.1 BLE is the end node. There are three end nodes configured in this applicaiton which means that we can track three assets at a time. 
