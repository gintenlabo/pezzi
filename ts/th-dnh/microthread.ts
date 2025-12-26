type GeneratorType = Generator<void, void, void>;

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

  private startTask<Args extends unknown[]>(taskFn: (...args: Args) => GeneratorType, ...args: Args): void {
    const gen = taskFn(...args);
    const { done } = gen.next();
    if (!done) {
      this.nextTasks.push(gen);
    }
  }

  static wrapTask<Args extends unknown[]>(taskFn: (...args: Args) => GeneratorType): (...args: Args) => void {
    return (...args) => {
      const manager = MicrothreadManager.runningManager;
      if (!manager) {
        throw new Error('task is called outside of microthreads; if you want to run task, use start()');
      }
      manager.startTask(taskFn, ...args);
    };
  }

  start<Args extends unknown[]>(task: (...args: Args) => void, ...args: Args): void {
    this.withContext(() => {
      task(...args);
    }, 'calling start in microthreads');
  }

  countTasks(): number {
    return this.nextTasks.length;
  }
}

export const wrapTask = <Args extends unknown[]>(taskFn: (...args: Args) => GeneratorType) =>
  MicrothreadManager.wrapTask(taskFn);
