import { NotifyGoal } from "../../../src/app/usecases/notify-goal";
import { IMatchStatus } from "../../../src/domain/entities/match.entity";
import { MqttTopics } from "../../../src/domain/constants/mqtt-topics.constants";
import { NotifyGoalUseCase } from "../../../src/app/usecases/notify-goal.usecase";

const makeMatch = (overrides = {}) => ({
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

const mockMqtt = {
  connect: jest.fn(),
  disconnect: jest.fn(),
  publish: jest.fn(),
  subscribe: jest.fn(),
  unsubscribe: jest.fn(),
};

describe("NotifyGoal", () => {
  let usecase: NotifyGoalUseCase;

  beforeEach(() => {
    jest.clearAllMocks();
    usecase = new NotifyGoal(mockMqtt as any);
  });

  it("publica no tópico correto quando há gol", async () => {
    const prev = [makeMatch({ homeScore: 0 })];
    const curr = [makeMatch({ homeScore: 1 })];

    await usecase.execute(prev, curr);

    expect(mockMqtt.publish).toHaveBeenCalledWith(
      MqttTopics.GOAL(1),
      expect.stringContaining('"scoringTeam":"home"'),
    );
  });

  it("retorna os gols detectados", async () => {
    const prev = [makeMatch({ homeScore: 0, awayScore: 0 })];
    const curr = [makeMatch({ homeScore: 1, awayScore: 0 })];

    const goals = await usecase.execute(prev, curr);

    expect(goals).toHaveLength(1);
    expect(goals[0].scoringTeam).toBe("home");
    expect(goals[0].homeScore).toBe(1);
  });

  it("não publica se não houver gol", async () => {
    const prev = [makeMatch({ homeScore: 1 })];
    const curr = [makeMatch({ homeScore: 1 })];

    await usecase.execute(prev, curr);

    expect(mockMqtt.publish).not.toHaveBeenCalled();
  });

  it("publica múltiplos gols em partidas diferentes", async () => {
    const prev = [
      makeMatch({ id: 1, homeScore: 0 }),
      makeMatch({ id: 2, homeScore: 0 }),
    ];
    const curr = [
      makeMatch({ id: 1, homeScore: 1 }),
      makeMatch({ id: 2, homeScore: 1 }),
    ];

    const goals = await usecase.execute(prev, curr);

    expect(goals).toHaveLength(2);
    expect(mockMqtt.publish).toHaveBeenCalledTimes(2);
    expect(mockMqtt.publish).toHaveBeenCalledWith(
      MqttTopics.GOAL(1),
      expect.any(String),
    );
    expect(mockMqtt.publish).toHaveBeenCalledWith(
      MqttTopics.GOAL(2),
      expect.any(String),
    );
  });
});
