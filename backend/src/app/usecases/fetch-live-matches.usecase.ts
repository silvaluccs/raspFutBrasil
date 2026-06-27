import { IMatch } from "../../domain/entities/match.entity";

export interface FetchLiveMatchesUseCase {
  execute(): Promise<IMatch[]>;
}

export const FetchLiveMatchesUseCase = Symbol("FetchLiveMatchesUseCase");
