export interface WeatherReading {
    id: number;
    timestamp: string;
    temperature: number;
    humidity: number;
    pressure: number;
}

export interface MetricStats {
    minimum: number;
    maximum: number;
    average: number;
}


export interface WeatherStats {
    temperature_stats: MetricStats;
    humidity_stats: MetricStats;
    pressure_stats: MetricStats;
}
