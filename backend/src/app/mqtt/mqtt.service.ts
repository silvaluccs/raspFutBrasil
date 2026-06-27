import mqtt, { MqttClient } from "mqtt";
import "dotenv/config";
import { MqttConnectionException } from "../../infra/shared/exceptions";
import { IMqttService } from "./mqtt.service.interface";

export class MqttService implements IMqttService {
  private client: MqttClient | null = null;
  private readonly brokerUrl: string;

  constructor() {
    this.brokerUrl = process.env.MQTT_BROKER_URL || "mqtt://localhost:1883";
  }

  async connect(): Promise<void> {
    return new Promise((resolve, reject) => {
      this.client = mqtt.connect(this.brokerUrl);

      this.client.on("connect", () => {
        console.log(`MQTT conectado em ${this.brokerUrl}`);
        resolve();
      });

      this.client.on("error", (err) => {
        console.error("MQTT erro:", err);
        reject(err);
      });
    });
  }

  async disconnect(): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return resolve();

      this.client.end(false, {}, (err) => {
        if (err) return reject(err);
        console.log("MQTT desconectado");
        resolve();
      });
    });
  }

  async publish(topic: string, message: string): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new MqttConnectionException());

      this.client.publish(topic, message, (err) => {
        if (err) return reject(err);
        resolve();
      });
    });
  }

  async subscribe(topic: string): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new MqttConnectionException());

      this.client.subscribe(topic, (err) => {
        if (err) return reject(err);
        console.log(`MQTT subscrito em: ${topic}`);
        resolve();
      });
    });
  }

  async unsubscribe(topic: string): Promise<void> {
    return new Promise((resolve, reject) => {
      if (!this.client) return reject(new MqttConnectionException());

      this.client.unsubscribe(topic, (err) => {
        if (err) return reject(err);
        resolve();
      });
    });
  }

  onMessage(callback: (topic: string, message: string) => void): void {
    if (!this.client) throw new Error("MQTT não conectado")
    this.client.on("message", (topic, payload) => {
      callback(topic, payload.toString())
    })
  }
}
