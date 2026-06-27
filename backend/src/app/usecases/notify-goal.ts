import { IMatch } from "../../domain/entities/match.entity";
import {
  GoalDetector,
  IGoalEvent,
} from "../../domain/services/goal-detector.service";
import { IMqttService } from "../mqtt/mqtt.service.interface";
import { MqttTopics } from "../../domain/constants/mqtt-topics.constants";
import { NotifyGoalUseCase } from "./notify-goal.usecase";

export class NotifyGoal implements NotifyGoalUseCase {
  private readonly goalDetector: GoalDetector;

  constructor(private readonly mqtt: IMqttService) {
    this.goalDetector = new GoalDetector();
  }

  async execute(previous: IMatch[], current: IMatch[]): Promise<IGoalEvent[]> {
    const goals = this.goalDetector.detect(previous, current);

    for (const goal of goals) {
      await this.mqtt.publish(
        MqttTopics.GOAL(goal.matchId),
        JSON.stringify(goal),
      );
    }

    return goals;
  }
}
