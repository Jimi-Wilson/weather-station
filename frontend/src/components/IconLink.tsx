import { Button } from "@/components/ui/button.tsx";
import { useMediaQuery } from "@/hooks/useMediaQuery";

type IconLinkProps = {
    url: string;
    text: string;
    lightIconUrl: string;
    darkIconUrl: string;
    altText: string;
    mode?: 'responsive' | 'iconOnly';
}

export function IconLink({
                             url,
                             text,
                             lightIconUrl,
                             darkIconUrl,
                             altText,
                             mode = 'iconOnly'
                         }: IconLinkProps) {
    const isDesktop = useMediaQuery("(min-width: 640px)");
    const showText = mode === 'responsive' && isDesktop;

    return (
        <Button
            variant="outline"
            size={showText ? "default" : "icon"}
            asChild
        >
            <a href={url} target="_blank" rel="noopener noreferrer" className="flex items-center">
                <img src={lightIconUrl} alt={altText} className="h-[1.2rem] w-[1.2rem] block dark:hidden" />
                <img src={darkIconUrl} alt={altText} className="h-[1.2rem] w-[1.2rem] hidden dark:block" />

                {showText && (
                    <span className="ml-2">
                        {text}
                    </span>
                )}
            </a>
        </Button>
    );
}