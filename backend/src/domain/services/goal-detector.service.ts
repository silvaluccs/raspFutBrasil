import { IMatch } from "../entities/match.entity";

export interface IGoalEvent {
  matchId: number;
  homeTeam: string;
  awayTeam: string;
  homeScore: number;
  awayScore: number;
  scoringTeam: "home" | "away";
}

export class GoalDetector {
  detect(previous: IMatch[], current: IMatch[]): IGoalEvent[] {
    const goals: IGoalEvent[] = [];

    for (const currentMatch of current) {
      const previousMatch = previous.find((m) => m.id === currentMatch.id);

      if (!previousMatch) continue;
      if (currentMatch.status !== previousMatch.status) continue;

      const homeScored = currentMatch.homeScore > previousMatch.homeScore;
      const awayScored = currentMatch.awayScore > previousMatch.awayScore;

      if (homeScored) {
        goals.push(this.makeGoalEvent(currentMatch, "home"));
      }

      if (awayScored) {
        goals.push(this.makeGoalEvent(currentMatch, "away"));
      }
    }

    return goals;
  }

  private makeGoalEvent(
    match: IMatch,
    scoringTeam: "home" | "away",
  ): IGoalEvent {
    return {
      matchId: match.id,
      homeTeam: match.shortHomeTeamName,
      awayTeam: match.shortAwayTeamName,
      homeScore: match.homeScore,
      awayScore: match.awayScore,
      scoringTeam,
    };
  }
}
