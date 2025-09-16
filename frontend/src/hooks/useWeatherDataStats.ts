import {useCallback, useEffect, useState} from "react";
import {weatherService} from "@/services/weatherService.ts";
import type {WeatherStats} from "@/services/types.ts";
import axios from "axios";

export const useWeatherDataStats = (hours: number = 24) => {
    const [data, setData] = useState<WeatherStats | null>(null);
    const [isLoading, setIsLoading] = useState(true);
    const [error, setError] = useState<string | null>(null);

    const fetchData = useCallback(
        async () => {
            try {
                if (!data) setIsLoading(true);
                setError(null);
                const weatherData = await weatherService.getWeatherDataStats(hours);
                setData(weatherData);
            } catch (err) {
                if (axios.isAxiosError(err)) {
                    const errorMessage = err.response?.data?.detail || err.message || 'Failed to fetch weather stats data';
                    setError(errorMessage);
                } else {
                    setError('An unexpected error occurred');
                }
            } finally {
                setIsLoading(false);
            }

        }, [hours]);


    useEffect(() => {
        fetchData();
    }, [fetchData]);



    return { data, isLoading, error };

}