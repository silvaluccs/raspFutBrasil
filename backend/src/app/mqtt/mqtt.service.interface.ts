export interface IMqttService {
  connect(): Promise<void>;
  disconnect(): Promise<void>;
  publish(topic: string, message: string): Promise<void>;
  subscribe(topic: string): Promise<void>;
  unsubscribe(topic: string): Promise<void>;
  onMessage(callback: (topic: string, message: string) => void): void;
}

export const IMqttService = Symbol("IMqttService");
