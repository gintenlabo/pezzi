type GeneratorType = Generator<void, void, void>;

interface Task<Args extends unknown[]> {
  (...args: Args): void;
  taskFn: (...args: Args) => GeneratorType;
}

export class MicrothreadManager {
  private nextTasks: Array<GeneratorType>;
  private static runningManager: MicrothreadManager | null = null;

  constructor() {
    this.nextTasks = [];
  }

  private withContext<T>(callback: (this: MicrothreadManager) => T, errorMsg?: string): T {
    if (MicrothreadManager.runningManager) {
      throw new Error(errorMsg ?? 'already in context');
    }
    MicrothreadManager.runningManager = this;
    try {
      return callback.call(this);
    } finally {
      MicrothreadManager.runningManager = null;
    }
  }

  yield(): boolean {
    return this.withContext(() => {
      const tasks = this.nextTasks;
      this.nextTasks = [];
      tasks.forEach((task) => {
        try {
          const { done } = task.next();
          if (!done) {
            this.nextTasks.push(task);
          }
        } catch (e) {
          console.error(e);
        }
      });
      return this.nextTasks.length > 0;
    }, 'running yield() in microthreads or another microthreads are running');
  }

  private callTaskFn<Args extends unknown[]>(taskFn: (...args: Args) => GeneratorType, ...args: Args): void {
    const gen = taskFn(...args);
    const { done } = gen.next();
    if (!done) {
      this.nextTasks.push(gen);
    }
  }

  static wrapTask<Args extends unknown[]>(taskFn: (...args: Args) => GeneratorType): Task<Args> {
    const result = (...args: Args) => {
      const manager = MicrothreadManager.runningManager;
      if (!manager) {
        throw new Error('task is called outside of microthreads; if you want to run, use callTask()');
      }
      manager.callTaskFn(taskFn, ...args);
    };
    result.taskFn = taskFn;
    return result;
  }

  startTask<Args extends unknown[]>(task: Task<Args>, ...args: Args): void {
    const { taskFn } = task;
    this.nextTasks.push(taskFn(...args));
  }

  callTask<Args extends unknown[]>(task: Task<Args>, ...args: Args): void {
    const { taskFn } = task;
    this.callTaskFn(taskFn, ...args);
  }
}

export const wrapTask = <Args extends unknown[]>(taskFn: (...args: Args) => GeneratorType) =>
  MicrothreadManager.wrapTask(taskFn);
