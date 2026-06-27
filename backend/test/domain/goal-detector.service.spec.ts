// test/domain/goal-detector.service.spec.ts
import { GoalDetector } from "../../src/domain/services/goal-detector.service";
import { IMatch, IMatchStatus } from "../../src/domain/entities/match.entity";

const makeMatch = (overrides = {}): IMatch => ({
  id: 1,
  shortHomeTeamName: "BRA",
  shortAwayTeamName: "JPN",
  homeScore: 0,
  homeColor: "#EAE380",
  awayScore: 0,
  awayColor: "#FFFFFF",
  status: IMatchStatus.LIVE,
  minute: 41,
  date: "26/06/2026",
  time: "21:00",
  ...overrides,
});

const detector = new GoalDetector();

describe("GoalDetector", () => {
  it("detecta gol do time da casa", () => {
    const prev = [makeMatch({ homeScore: 0, awayScore: 0 })];
    const curr = [makeMatch({ homeScore: 1, awayScore: 0 })];

    const goals = detector.detect(prev, curr);

    expect(goals).toHaveLength(1);
    expect(goals[0].scoringTeam).toBe("home");
  });

  it("detecta gol do visitante", () => {
    const prev = [makeMatch({ homeScore: 0, awayScore: 0 })];
    const curr = [makeMatch({ homeScore: 0, awayScore: 1 })];

    expect(detector.detect(prev, curr)[0].scoringTeam).toBe("away");
  });

  it("detecta gols simultâneos em partidas diferentes", () => {
    const prev = [
      makeMatch({ id: 1, homeScore: 0 }),
      makeMatch({ id: 2, homeScore: 0 }),
    ];
    const curr = [
      makeMatch({ id: 1, homeScore: 1 }),
      makeMatch({ id: 2, homeScore: 1 }),
    ];

    expect(detector.detect(prev, curr)).toHaveLength(2);
  });

  it("não detecta gol se placar igual", () => {
    const prev = [makeMatch({ homeScore: 1, awayScore: 0 })];
    const curr = [makeMatch({ homeScore: 1, awayScore: 0 })];

    expect(detector.detect(prev, curr)).toHaveLength(0);
  });

  it("ignora partida nova sem histórico", () => {
    const prev: IMatch[] = [];
    const curr = [makeMatch({ homeScore: 1 })];

    expect(detector.detect(prev, curr)).toHaveLength(0);
  });
});
