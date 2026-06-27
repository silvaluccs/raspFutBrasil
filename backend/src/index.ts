import { MqttService } from "./app/mqtt/mqtt.service";
import { IMqttService } from "./app/mqtt/mqtt.service.interface";
import { WorldCupResultGateway } from "./infra/gateways/world-cup-results.gateway";

const worldCupResultsGateway = new WorldCupResultGateway();

async function main() {
  const mqttService: IMqttService = new MqttService();
  await mqttService.connect();
}

main();
