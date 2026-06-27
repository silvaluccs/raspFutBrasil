import { IMatch } from "../../domain/entities/match.entity";
import { IGoalEvent } from "../../domain/services/goal-detector.service";

export interface NotifyGoalUseCase {
  execute(previous: IMatch[], current: IMatch[]): Promise<IGoalEvent[]>;
}

export const NotifyGoalUseCase = Symbol("NotifyGoalUseCase");
