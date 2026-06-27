import { MqttTopics } from "../../domain/constants/mqtt-topics.constants";
import { IMatch, IMatchStatus } from "../../domain/entities/match.entity";
import { IWorldCupResultsGateway } from "../../infra/gateways/world-cup-results.gateway.interface";
import { IMqttService } from "../mqtt/mqtt.service.interface";
import { FetchLiveMatchesUseCase } from "./fetch-live-matches.usecase";

export class FetchLiveMatches implements FetchLiveMatchesUseCase {
  constructor(
    private readonly gateway: IWorldCupResultsGateway,
    private readonly mqtt: IMqttService,
  ) {}

  async execute(): Promise<IMatch[]> {
    const matches = await this.gateway.getAllMatchToday();
    const liveMatches = matches.filter((m) => m.status === IMatchStatus.LIVE);

    await this.mqtt.publish(MqttTopics.TODAY, JSON.stringify(liveMatches));

    for (const match of liveMatches) {
      await this.mqtt.publish(
        MqttTopics.DETAILS(match.id),
        JSON.stringify(match),
      );
    }

    return liveMatches;
  }
}
